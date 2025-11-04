# BranchCloak

> **BranchCloak: Mitigating Side-Channel Attacks on Directional Branch Predictors**  
> *Jihoon Kim, Hyerean Jang, and Youngjoo Shin*  
> *School of Cybersecurity, Korea University*  
> Published in **Electronics 2025, 14(9):1758**  
> [https://doi.org/10.3390/electronics14091758](https://doi.org/10.3390/electronics14091758)

---

## 🧩 Overview

**BranchCloak** is a novel **software-based mitigation technique** that protects processors from **Pattern History Table (PHT)** side-channel attacks on **directional branch predictors**.  
Instead of modifying hardware, BranchCloak obfuscates the PHT state by inserting *r-branches*—randomized dummy conditional branches—that collide with the target branch in the PHT.  
This randomization prevents attackers from inferring the branch direction of victim processes.

---

## ⚙️ Core Idea

PHT-based attacks (e.g., **BranchScope**, **Bluethunder**) exploit shared branch predictor state between cores.  
BranchCloak neutralizes this by ensuring:

- Each **r-branch**:
  1. Occupies the *same PHT entry* as the target (secret-dependent) branch.  
  2. Makes a *random branch decision* (50% taken / not-taken).

Even if an attacker probes the PHT, the randomized state reveals no information about the victim’s control flow.

---

## 🔬 Key Contributions

- **Software-only mitigation** requiring no hardware modification.  
- **Reverse engineering of Intel’s PHT structure** to discover that 15-bit address alignment (via `.p2align 15`) ensures collision in the same PHT entry.  
- **Formal probabilistic and information-theoretic proof** of *perfect security* (mutual information = 0).  
- **Customized GCC compiler extension** supporting directives:

  ```c
  SECURE_start:
      if (secret) { ... }
  SECURE_end:
  ```

  → automatically inserts two aligned *r-branches*.

---

## 📊 Evaluation

- Tested on **Intel Kaby Lake, Comet Lake, Rocket Lake** CPUs  
- Benchmarks: **OpenSSL 3.1.0**, **MbedTLS 3.1.0**, **Libgcrypt 1.9.4**  
- Targeted secret-dependent branches (e.g., RSA modular exponentiation, ECC scalar multiplication)  

---

## 📚 Citation

If you use BranchCloak in academic work, please cite:

[[[
@article{kim2025branchcloak,
  title   = {BranchCloak: Mitigating Side-Channel Attacks on Directional Branch Predictors},
  author  = {Kim, Jihoon and Jang, Hyerean and Shin, Youngjoo},
  journal = {Electronics},
  volume  = {14},
  number  = {9},
  pages   = {1758},
  year    = {2025},
  doi     = {10.3390/electronics14091758}
}
]]]
