# SPDX-License-Identifier: Apache-2.0

include ../Makefile_params.common

HARNESS_ENTRY = harness
HARNESS_FILE = demos_harness

# This should be a unique identifier for this proof, and will appear on the
# Litani dashboard. It can be human-readable and contain spaces if you wish.
PROOF_UID = demo_multiply_arrays

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROJECT_SOURCES += $(SRCDIR)/proofs/cbmc/demos/demos.c

CHECK_FUNCTION_CONTRACTS=demo_multiply_arrays
USE_FUNCTION_CONTRACTS=demo_multiply
APPLY_LOOP_CONTRACTS=on
USE_DYNAMIC_FRAMES=1

# Disable any setting of EXTERNAL_SAT_SOLVER, and choose SMT backend instead
EXTERNAL_SAT_SOLVER=
CBMCFLAGS=--bitwuzla

FUNCTION_NAME = demo_multiply_arrays

# This function is large enough to need...
CBMC_OBJECT_BITS = 8

include ../Makefile.common
