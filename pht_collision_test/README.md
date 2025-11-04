# Implementing PHT Side-Channel Attacks and Applying BranchCloak

This document provides a clear, GitHub-ready README describing how to reproduce Pattern History Table (PHT) based side-channel attacks (e.g., BranchScope) and how to test BranchCloak protections against those attacks.

---

## Prerequisites

- A Linux machine with `perf` (or equivalent performance-counter tooling) installed.
- `make` build system available.
- `taskset` (to pin processes to specific CPU cores).
- BranchCloak-enabled compiler or source modifications included in the project repository.

---

## PHT entry collision test

Build and run the PHT collision test:

```bash
make attack
taskset -c <cpuid> ./test.out
```

---

## 1-bit leakage attack (BranchScope[1])

Build and run the 1-bit leakage test:

```bash
make bit
taskset -c <cpuid> ./test.out
```

### Notes

- Before running the attack, use `/utils/generate_rand.py` to generate the PHT randomization code that fixes the PHT entry state to `WT`/`ST` for the target architecture. This step ensures the attack starts from a predictable PHT initial state.


---

## 1-bit leakage attack with BranchCloak applied

Build and run the protected binary compiled with the BranchCloak-enabled toolchain (for example, using `./as_custom/gcc.out` and `./as_custom/as.out`):

```bash
make safe
taskset -c <cpuid> ./test.out
```

- `make safe` should produce a binary containing the inserted r-branches that obfuscate the PHT state.
- If BranchCloak is applied in the code base, you will see a protection stub like:

```c
if (rand() % 2) { asm volatile("nop;" ::: "rax", "rdx"); } /* SAFE_BRANCH */
```

To run the original (unprotected) attack scenario, remove this code and recompile the victim binary (i.e., remove the r-branch).

---

<small><em>[1] Evtyushkin, Dmitry, et al. "Branchscope: A new side-channel attack on directional branch predictor." ACM SIGPLAN Notices 53.2 (2018): 693-707.</em></small>