# PHT Randomization Code Generator

This tool generates a randomized sequence of conditional branches that **fix a PHT entry** to a specified saturating-counter state (SN / WN / WT / ST). The generated code (`randomize_pht.c`) is intended to be used by the attack code in `/pht_collision_test/`.

---

## Usage

### 1) Build the PHT randomization code checker

First compile the PHT randomization *checker* that verifies the effect of the generated code:

```bash
make all
```

- The checker runs the generated PHT randomization sequence and reports the observed PHT state. It can confirm that the PHT entry is fixed to the intended state or indicate that the result is inconsistent on the target CPU.

### 2) Generate PHT randomization code

Run the generator (Python 3) to produce a randomized conditional-branch sequence that aims to drive a PHT entry to a chosen saturating-counter state:

```bash
python3 generate_rand.py <SN|WN|WT|ST>
```

- Argument selects the target PHT state to force:
  - `SN` — Strongly Not-taken
  - `WN` — Weakly Not-taken
  - `WT` — Weakly Taken
  - `ST` — Strongly Taken

- The script produces a file named `randomize_pht.c` that contains a sequence of randomized conditional branches designed to drive the target PHT entry into the chosen state on the target architecture.

---

## Notes

- The exact number and pattern of branches required to reliably set the PHT entry depends on the target CPU microarchitecture and predictor behavior. Verify the produced code on the target machine.
