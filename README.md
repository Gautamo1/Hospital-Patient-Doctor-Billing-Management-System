# Health-Management-System

A C-based Hospital Management System (HMS) for managing patients, doctors, services, appointments, and billing.

## Features

- **Patient Management** — Add, view, and manage patient records
- **Doctor Management** — Manage doctor profiles and availability
- **Service Management** — Define hospital services and pricing
- **Appointments** — Schedule and track patient appointments
- **Billing** — Generate bills and track bill items

## Project Structure

```
hms_project/
├── src/              # Source code files (.c)
├── include/          # Header files (.h)
├── obj/              # Compiled object files (.o) [build output]
├── bin/              # Final executable [build output]
├── data/             # Runtime data files (.dat) [generated at runtime]
├── tools/            # Utility scripts (dummy data generator)
├── .vscode/          # VS Code settings and tasks
├── Makefile          # Build configuration
├── build.ps1         # PowerShell build script (Windows)
└── README.md         # This file
```

## Prerequisites

- **GCC compiler** (MinGW on Windows, or via MSYS2/WSL)
- **Make** (optional; use PowerShell build script as alternative on Windows)
- **Git** (for version control)

### Windows Setup

If you don't have a compiler:

1. **Install MSYS2** from https://www.msys2.org/
2. Open "MSYS2 MinGW 64-bit" shell and run:
   ```bash
   pacman -Syu
   # Reopen shell if asked, then:
   pacman -S --needed base-devel mingw-w64-x86_64-toolchain
   ```
3. Verify gcc and make are installed:
   ```bash
   gcc --version
   make --version
   ```

## Building the Project

### Option 1: Using Make (Recommended if installed)

```bash
# MSYS2 MinGW64 shell, Git Bash, or Linux/WSL
make
./bin/hms_project
```

### Option 2: Using PowerShell Build Script (Windows, no Make needed)

```powershell
# From project root in PowerShell
.\build.ps1 -Action build
.\bin\hms_project.exe
```

### Option 3: Using VS Code

- Open the project folder in VS Code
- Press **Ctrl+Shift+B** to run the default build task
- Or use **Ctrl+Shift+D** to run and debug (pick a launch config)

### Clean Build

```bash
make clean          # if using Make
# or
.\build.ps1 -Action clean   # if using PowerShell script
```

## Running the Application

After building, run:

```bash
# Linux/macOS/WSL/MSYS2 shell
./bin/hms_project

# Windows PowerShell
.\bin\hms_project.exe
```

The application presents a menu-driven interface:
```
===== Hospital Management System =====
1. Patient Management
2. Doctor Management
3. Service Management
4. Appointment Management
5. Billing Management
6. Exit

Enter your choice:
```

### Sample Data

The project includes dummy data (patients, doctors, services, appointments, bills) pre-populated in `data/*.dat` files. To regenerate dummy data:

```bash
# From project root (MSYS2/Bash or PowerShell)
cd tools
gcc -std=c11 generate_dummy_data.c -o gen_dummy.exe
./gen_dummy.exe
# Data files are recreated in ../data/
```

## File Descriptions

| File | Purpose |
|------|---------|
| `src/main.c` | Main entry point and menu loop |
| `src/patient.c` | Patient CRUD operations |
| `src/doctor.c` | Doctor management |
| `src/service.c` | Service definitions |
| `src/appointment.c` | Appointment scheduling |
| `src/billing.c` | Billing and invoicing |
| `src/input.c` | Input validation utilities |
| `include/common.h` | Data structures and constants |
| `Makefile` | Build rules (compiles to `obj/`, outputs to `bin/`) |
| `build.ps1` | PowerShell build wrapper (Windows) |

## Compilation Details

- **Compiler Flags:** `-Wall -Wextra -Werror -std=c11 -Iinclude`
- **Object Files:** Stored in `obj/` directory
- **Executable:** `bin/hms_project` (or `bin/hms_project.exe` on Windows)
- **Data Storage:** Binary files in `data/` (patients.dat, doctors.dat, etc.)

## Troubleshooting

### "make: command not found"
- Install make via MSYS2 (recommended) or Chocolatey, or use `.\build.ps1` instead.

### "Failed to open patient file: No such file or directory"
- The `data/` directory must exist. It's created automatically; if missing, regenerate dummy data:
  ```bash
  cd tools && gcc -std=c11 generate_dummy_data.c -o gen_dummy.exe && ./gen_dummy.exe
  ```

### Compiler errors on Windows PowerShell
- Use MSYS2 MinGW64 shell instead, or ensure gcc is on PATH.

## Development Notes

- Compiled object files (`.o`) are stored separately in `obj/` for clean project organization.
- All data is persisted in binary format in `data/*.dat` files.
- The system uses fixed-size structs for binary I/O; see `include/common.h` for struct definitions.

## License

This project is part of a hospital management system coursework.