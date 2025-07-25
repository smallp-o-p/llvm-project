; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 5

; RUN: llc < %s -disable-wasm-fallthrough-return-opt -wasm-disable-explicit-locals -wasm-keep-registers  -mattr=+fp16,+simd128,+relaxed-simd | FileCheck %s --check-prefix=RELAXED
; RUN: llc < %s -disable-wasm-fallthrough-return-opt -wasm-disable-explicit-locals -wasm-keep-registers  -mattr=+fp16,+simd128,              | FileCheck %s --check-prefix=STRICT

target triple = "wasm32"

define double @fadd_fmul_contract_f64(double %a, double %b, double %c) {
; RELAXED-LABEL: fadd_fmul_contract_f64:
; RELAXED:         .functype fadd_fmul_contract_f64 (f64, f64, f64) -> (f64)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f64.mul $push0=, $1, $0
; RELAXED-NEXT:    f64.add $push1=, $pop0, $2
; RELAXED-NEXT:    return $pop1
;
; STRICT-LABEL: fadd_fmul_contract_f64:
; STRICT:         .functype fadd_fmul_contract_f64 (f64, f64, f64) -> (f64)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f64.mul $push0=, $1, $0
; STRICT-NEXT:    f64.add $push1=, $pop0, $2
; STRICT-NEXT:    return $pop1
  %mul = fmul contract double %b, %a
  %add = fadd contract double %mul, %c
  ret double %add
}

define <4 x float> @fadd_fmul_contract_4xf32(<4 x float> %a, <4 x float> %b, <4 x float> %c) {
; RELAXED-LABEL: fadd_fmul_contract_4xf32:
; RELAXED:         .functype fadd_fmul_contract_4xf32 (v128, v128, v128) -> (v128)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f32x4.relaxed_madd $push0=, $2, $1, $0
; RELAXED-NEXT:    return $pop0
;
; STRICT-LABEL: fadd_fmul_contract_4xf32:
; STRICT:         .functype fadd_fmul_contract_4xf32 (v128, v128, v128) -> (v128)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f32x4.mul $push0=, $1, $0
; STRICT-NEXT:    f32x4.add $push1=, $pop0, $2
; STRICT-NEXT:    return $pop1
  %mul = fmul contract <4 x float> %b, %a
  %add = fadd contract <4 x float> %mul, %c
  ret <4 x float> %add
}


define <8 x half> @fadd_fmul_contract_8xf16(<8 x half> %a, <8 x half> %b, <8 x half> %c) {
; RELAXED-LABEL: fadd_fmul_contract_8xf16:
; RELAXED:         .functype fadd_fmul_contract_8xf16 (v128, v128, v128) -> (v128)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f16x8.relaxed_madd $push0=, $2, $1, $0
; RELAXED-NEXT:    return $pop0
;
; STRICT-LABEL: fadd_fmul_contract_8xf16:
; STRICT:         .functype fadd_fmul_contract_8xf16 (v128, v128, v128) -> (v128)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f16x8.mul $push0=, $1, $0
; STRICT-NEXT:    f16x8.add $push1=, $pop0, $2
; STRICT-NEXT:    return $pop1
  %mul = fmul contract <8 x half> %b, %a
  %add = fadd contract <8 x half> %mul, %c
  ret <8 x half> %add
}


define <4 x float> @fadd_fmul_4xf32(<4 x float> %a, <4 x float> %b, <4 x float> %c) {
; RELAXED-LABEL: fadd_fmul_4xf32:
; RELAXED:         .functype fadd_fmul_4xf32 (v128, v128, v128) -> (v128)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f32x4.mul $push0=, $1, $0
; RELAXED-NEXT:    f32x4.add $push1=, $pop0, $2
; RELAXED-NEXT:    return $pop1
;
; STRICT-LABEL: fadd_fmul_4xf32:
; STRICT:         .functype fadd_fmul_4xf32 (v128, v128, v128) -> (v128)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f32x4.mul $push0=, $1, $0
; STRICT-NEXT:    f32x4.add $push1=, $pop0, $2
; STRICT-NEXT:    return $pop1
  %mul = fmul <4 x float> %b, %a
  %add = fadd contract <4 x float> %mul, %c
  ret <4 x float> %add
}

define <4 x float> @fmuladd_contract_4xf32(<4 x float> %a, <4 x float> %b, <4 x float> %c) {
; RELAXED-LABEL: fmuladd_contract_4xf32:
; RELAXED:         .functype fmuladd_contract_4xf32 (v128, v128, v128) -> (v128)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f32x4.relaxed_madd $push0=, $2, $0, $1
; RELAXED-NEXT:    return $pop0
;
; STRICT-LABEL: fmuladd_contract_4xf32:
; STRICT:         .functype fmuladd_contract_4xf32 (v128, v128, v128) -> (v128)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f32x4.mul $push0=, $0, $1
; STRICT-NEXT:    f32x4.add $push1=, $pop0, $2
; STRICT-NEXT:    return $pop1
  %fma = call contract <4 x float> @llvm.fmuladd(<4 x float> %a, <4 x float> %b, <4 x float> %c)
  ret <4 x float> %fma
}

; TODO: This should also have relaxed_madd in RELAXED case
define <4 x float> @fmuladd_4xf32(<4 x float> %a, <4 x float> %b, <4 x float> %c) {
; RELAXED-LABEL: fmuladd_4xf32:
; RELAXED:         .functype fmuladd_4xf32 (v128, v128, v128) -> (v128)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f32x4.mul $push0=, $0, $1
; RELAXED-NEXT:    f32x4.add $push1=, $pop0, $2
; RELAXED-NEXT:    return $pop1
;
; STRICT-LABEL: fmuladd_4xf32:
; STRICT:         .functype fmuladd_4xf32 (v128, v128, v128) -> (v128)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f32x4.mul $push0=, $0, $1
; STRICT-NEXT:    f32x4.add $push1=, $pop0, $2
; STRICT-NEXT:    return $pop1
  %fma = call <4 x float> @llvm.fmuladd(<4 x float> %a, <4 x float> %b, <4 x float> %c)
  ret <4 x float> %fma
}

define <4 x float> @fma_4xf32(<4 x float> %a, <4 x float> %b, <4 x float> %c) {
; RELAXED-LABEL: fma_4xf32:
; RELAXED:         .functype fma_4xf32 (v128, v128, v128) -> (v128)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f32x4.extract_lane $push2=, $0, 0
; RELAXED-NEXT:    f32x4.extract_lane $push1=, $1, 0
; RELAXED-NEXT:    f32x4.extract_lane $push0=, $2, 0
; RELAXED-NEXT:    call $push3=, fmaf, $pop2, $pop1, $pop0
; RELAXED-NEXT:    f32x4.splat $push4=, $pop3
; RELAXED-NEXT:    f32x4.extract_lane $push7=, $0, 1
; RELAXED-NEXT:    f32x4.extract_lane $push6=, $1, 1
; RELAXED-NEXT:    f32x4.extract_lane $push5=, $2, 1
; RELAXED-NEXT:    call $push8=, fmaf, $pop7, $pop6, $pop5
; RELAXED-NEXT:    f32x4.replace_lane $push9=, $pop4, 1, $pop8
; RELAXED-NEXT:    f32x4.extract_lane $push12=, $0, 2
; RELAXED-NEXT:    f32x4.extract_lane $push11=, $1, 2
; RELAXED-NEXT:    f32x4.extract_lane $push10=, $2, 2
; RELAXED-NEXT:    call $push13=, fmaf, $pop12, $pop11, $pop10
; RELAXED-NEXT:    f32x4.replace_lane $push14=, $pop9, 2, $pop13
; RELAXED-NEXT:    f32x4.extract_lane $push17=, $0, 3
; RELAXED-NEXT:    f32x4.extract_lane $push16=, $1, 3
; RELAXED-NEXT:    f32x4.extract_lane $push15=, $2, 3
; RELAXED-NEXT:    call $push18=, fmaf, $pop17, $pop16, $pop15
; RELAXED-NEXT:    f32x4.replace_lane $push19=, $pop14, 3, $pop18
; RELAXED-NEXT:    return $pop19
;
; STRICT-LABEL: fma_4xf32:
; STRICT:         .functype fma_4xf32 (v128, v128, v128) -> (v128)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f32x4.extract_lane $push2=, $0, 0
; STRICT-NEXT:    f32x4.extract_lane $push1=, $1, 0
; STRICT-NEXT:    f32x4.extract_lane $push0=, $2, 0
; STRICT-NEXT:    call $push3=, fmaf, $pop2, $pop1, $pop0
; STRICT-NEXT:    f32x4.splat $push4=, $pop3
; STRICT-NEXT:    f32x4.extract_lane $push7=, $0, 1
; STRICT-NEXT:    f32x4.extract_lane $push6=, $1, 1
; STRICT-NEXT:    f32x4.extract_lane $push5=, $2, 1
; STRICT-NEXT:    call $push8=, fmaf, $pop7, $pop6, $pop5
; STRICT-NEXT:    f32x4.replace_lane $push9=, $pop4, 1, $pop8
; STRICT-NEXT:    f32x4.extract_lane $push12=, $0, 2
; STRICT-NEXT:    f32x4.extract_lane $push11=, $1, 2
; STRICT-NEXT:    f32x4.extract_lane $push10=, $2, 2
; STRICT-NEXT:    call $push13=, fmaf, $pop12, $pop11, $pop10
; STRICT-NEXT:    f32x4.replace_lane $push14=, $pop9, 2, $pop13
; STRICT-NEXT:    f32x4.extract_lane $push17=, $0, 3
; STRICT-NEXT:    f32x4.extract_lane $push16=, $1, 3
; STRICT-NEXT:    f32x4.extract_lane $push15=, $2, 3
; STRICT-NEXT:    call $push18=, fmaf, $pop17, $pop16, $pop15
; STRICT-NEXT:    f32x4.replace_lane $push19=, $pop14, 3, $pop18
; STRICT-NEXT:    return $pop19
  %fma = call <4 x float> @llvm.fma(<4 x float> %a, <4 x float> %b, <4 x float> %c)
  ret <4 x float> %fma
}


define <8 x float> @fadd_fmul_contract_8xf32(<8 x float> %a, <8 x float> %b, <8 x float> %c) {
; RELAXED-LABEL: fadd_fmul_contract_8xf32:
; RELAXED:         .functype fadd_fmul_contract_8xf32 (i32, v128, v128, v128, v128, v128, v128) -> ()
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f32x4.relaxed_madd $push0=, $6, $4, $2
; RELAXED-NEXT:    v128.store 16($0), $pop0
; RELAXED-NEXT:    f32x4.relaxed_madd $push1=, $5, $3, $1
; RELAXED-NEXT:    v128.store 0($0), $pop1
; RELAXED-NEXT:    return
;
; STRICT-LABEL: fadd_fmul_contract_8xf32:
; STRICT:         .functype fadd_fmul_contract_8xf32 (i32, v128, v128, v128, v128, v128, v128) -> ()
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f32x4.mul $push0=, $4, $2
; STRICT-NEXT:    f32x4.add $push1=, $pop0, $6
; STRICT-NEXT:    v128.store 16($0), $pop1
; STRICT-NEXT:    f32x4.mul $push2=, $3, $1
; STRICT-NEXT:    f32x4.add $push3=, $pop2, $5
; STRICT-NEXT:    v128.store 0($0), $pop3
; STRICT-NEXT:    return
  %mul = fmul contract <8 x float> %b, %a
  %add = fadd contract <8 x float> %mul, %c
  ret <8 x float> %add
}


define <2 x double> @fadd_fmul_contract_2xf64(<2 x double> %a, <2 x double> %b, <2 x double> %c) {
; RELAXED-LABEL: fadd_fmul_contract_2xf64:
; RELAXED:         .functype fadd_fmul_contract_2xf64 (v128, v128, v128) -> (v128)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f64x2.relaxed_madd $push0=, $2, $1, $0
; RELAXED-NEXT:    return $pop0
;
; STRICT-LABEL: fadd_fmul_contract_2xf64:
; STRICT:         .functype fadd_fmul_contract_2xf64 (v128, v128, v128) -> (v128)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f64x2.mul $push0=, $1, $0
; STRICT-NEXT:    f64x2.add $push1=, $pop0, $2
; STRICT-NEXT:    return $pop1
  %mul = fmul contract <2 x double> %b, %a
  %add = fadd contract <2 x double> %mul, %c
  ret <2 x double> %add
}

define float @fadd_fmul_contract_f32(float %a, float %b, float %c) {
; RELAXED-LABEL: fadd_fmul_contract_f32:
; RELAXED:         .functype fadd_fmul_contract_f32 (f32, f32, f32) -> (f32)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    f32.mul $push0=, $1, $0
; RELAXED-NEXT:    f32.add $push1=, $pop0, $2
; RELAXED-NEXT:    return $pop1
;
; STRICT-LABEL: fadd_fmul_contract_f32:
; STRICT:         .functype fadd_fmul_contract_f32 (f32, f32, f32) -> (f32)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    f32.mul $push0=, $1, $0
; STRICT-NEXT:    f32.add $push1=, $pop0, $2
; STRICT-NEXT:    return $pop1
  %mul = fmul contract float %b, %a
  %add = fadd contract float %mul, %c
  ret float %add
}

define float @fma_f32(float %a, float %b, float %c) {
; RELAXED-LABEL: fma_f32:
; RELAXED:         .functype fma_f32 (f32, f32, f32) -> (f32)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    call $push0=, fmaf, $0, $1, $2
; RELAXED-NEXT:    return $pop0
;
; STRICT-LABEL: fma_f32:
; STRICT:         .functype fma_f32 (f32, f32, f32) -> (f32)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    call $push0=, fmaf, $0, $1, $2
; STRICT-NEXT:    return $pop0
  %fma = call float @llvm.fma(float %a, float %b, float %c)
  ret float %fma
}

define double @fma_f64(double %a, double %b, double %c) {
; RELAXED-LABEL: fma_f64:
; RELAXED:         .functype fma_f64 (f64, f64, f64) -> (f64)
; RELAXED-NEXT:  # %bb.0:
; RELAXED-NEXT:    call $push0=, fma, $0, $1, $2
; RELAXED-NEXT:    return $pop0
;
; STRICT-LABEL: fma_f64:
; STRICT:         .functype fma_f64 (f64, f64, f64) -> (f64)
; STRICT-NEXT:  # %bb.0:
; STRICT-NEXT:    call $push0=, fma, $0, $1, $2
; STRICT-NEXT:    return $pop0
  %fma = call double @llvm.fma(double %a, double %b, double %c)
  ret double %fma
}
