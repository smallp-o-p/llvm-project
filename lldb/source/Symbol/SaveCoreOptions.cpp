//===-- SaveCoreOptions.cpp -------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "lldb/Symbol/SaveCoreOptions.h"
#include "lldb/Core/PluginManager.h"
#include "lldb/Target/Process.h"
#include "lldb/Target/Thread.h"

using namespace lldb;
using namespace lldb_private;

Status SaveCoreOptions::SetPluginName(const char *name) {
  Status error;
  if (!name || !name[0]) {
    m_plugin_name = std::nullopt;
    return error;
  }

  std::vector<llvm::StringRef> plugin_names =
      PluginManager::GetSaveCorePluginNames();
  if (!llvm::is_contained(plugin_names, name)) {
    StreamString stream;
    stream.Printf("plugin name '%s' is not a valid ObjectFile plugin name.",
                  name);

    if (!plugin_names.empty()) {
      stream.PutCString(" Valid names are: ");
      std::string plugin_names_str = llvm::join(plugin_names, ", ");
      stream.PutCString(plugin_names_str);
      stream.PutChar('.');
    }
    return Status(stream.GetString().str());
  }

  m_plugin_name = name;
  return error;
}

void SaveCoreOptions::SetStyle(lldb::SaveCoreStyle style) { m_style = style; }

void SaveCoreOptions::SetOutputFile(FileSpec file) { m_file = file; }

std::optional<std::string> SaveCoreOptions::GetPluginName() const {
  return m_plugin_name;
}

lldb::SaveCoreStyle SaveCoreOptions::GetStyle() const {
  return m_style.value_or(lldb::eSaveCoreUnspecified);
}

const std::optional<lldb_private::FileSpec>
SaveCoreOptions::GetOutputFile() const {
  return m_file;
}

Status SaveCoreOptions::SetProcess(lldb::ProcessSP process_sp) {
  Status error;
  if (!process_sp) {
    ClearProcessSpecificData();
    m_process_sp.reset();
    return error;
  }

  if (!process_sp->IsValid()) {
    error = Status::FromErrorString("Cannot assign an invalid process.");
    return error;
  }

  // Don't clear any process specific data if the process is the same.
  if (m_process_sp == process_sp)
    return error;

  ClearProcessSpecificData();
  m_process_sp = process_sp;
  return error;
}

Status SaveCoreOptions::AddThread(lldb::ThreadSP thread_sp) {
  Status error;
  if (!thread_sp) {
    error = Status::FromErrorString("invalid thread");
    return error;
  }

  if (m_process_sp) {
    if (m_process_sp != thread_sp->GetProcess()) {
      error = Status::FromErrorString(
          "Cannot add a thread from a different process.");
      return error;
    }
  } else {
    m_process_sp = thread_sp->GetProcess();
  }

  m_threads_to_save.insert(thread_sp->GetID());
  return error;
}

bool SaveCoreOptions::RemoveThread(lldb::ThreadSP thread_sp) {
  return thread_sp && m_threads_to_save.erase(thread_sp->GetID()) > 0;
}

bool SaveCoreOptions::ShouldThreadBeSaved(lldb::tid_t tid) const {
  // If the user specified no threads to save, then we save all threads.
  if (m_threads_to_save.empty())
    return true;
  return m_threads_to_save.count(tid) > 0;
}

bool SaveCoreOptions::HasSpecifiedThreads() const {
  return !m_threads_to_save.empty();
}

void SaveCoreOptions::AddMemoryRegionToSave(
    const lldb_private::MemoryRegionInfo &region) {
  m_regions_to_save.Insert(region.GetRange(), /*combine=*/true);
}

const MemoryRanges &SaveCoreOptions::GetCoreFileMemoryRanges() const {
  return m_regions_to_save;
}
Status SaveCoreOptions::EnsureValidConfiguration() const {
  Status error;
  std::string error_str;
  if (!m_threads_to_save.empty() && GetStyle() == lldb::eSaveCoreFull)
    error_str += "Cannot save a full core with a subset of threads\n";

  if (!m_process_sp)
    error_str += "Need to assign a valid process\n";

  if (!error_str.empty())
    error = Status(error_str);

  return error;
}

lldb_private::ThreadCollection::collection
SaveCoreOptions::GetThreadsToSave() const {
  lldb_private::ThreadCollection::collection thread_collection;
  // In cases where no process is set, such as when no threads are specified.
  if (!m_process_sp)
    return thread_collection;

  ThreadList &thread_list = m_process_sp->GetThreadList();
  for (const auto &tid : m_threads_to_save)
    thread_collection.push_back(thread_list.FindThreadByID(tid));

  return thread_collection;
}

llvm::Expected<lldb_private::CoreFileMemoryRanges>
SaveCoreOptions::GetMemoryRegionsToSave() {
  Status error;
  if (!m_process_sp)
    return Status::FromErrorString("Requires a process to be set.").takeError();

  error = EnsureValidConfiguration();
  if (error.Fail())
    return error.takeError();

  CoreFileMemoryRanges ranges;
  error = m_process_sp->CalculateCoreFileSaveRanges(*this, ranges);
  if (error.Fail())
    return error.takeError();

  return ranges;
}

llvm::Expected<uint64_t> SaveCoreOptions::GetCurrentSizeInBytes() {
  Status error;
  if (!m_process_sp)
    return Status::FromErrorString("Requires a process to be set.").takeError();

  error = EnsureValidConfiguration();
  if (error.Fail())
    return error.takeError();

  CoreFileMemoryRanges ranges;
  error = m_process_sp->CalculateCoreFileSaveRanges(*this, ranges);
  if (error.Fail())
    return error.takeError();

  llvm::Expected<lldb_private::CoreFileMemoryRanges> core_file_ranges_maybe =
      GetMemoryRegionsToSave();
  if (!core_file_ranges_maybe)
    return core_file_ranges_maybe.takeError();
  const lldb_private::CoreFileMemoryRanges &core_file_ranges =
      *core_file_ranges_maybe;
  uint64_t total_in_bytes = 0;
  for (const auto &core_range : core_file_ranges)
    total_in_bytes += core_range.data.range.size();

  return total_in_bytes;
}

void SaveCoreOptions::ClearProcessSpecificData() {
  // Deliberately not following the formatter style here to indicate that
  // this method will be expanded in the future.
  m_threads_to_save.clear();
}

void SaveCoreOptions::Clear() {
  m_file = std::nullopt;
  m_plugin_name = std::nullopt;
  m_style = std::nullopt;
  m_threads_to_save.clear();
  m_process_sp.reset();
  m_regions_to_save.Clear();
}
