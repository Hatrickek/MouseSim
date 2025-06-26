# MouseSim
Mouse steering with pedals built for sim racing using vJoy.

# Usage
- Download built release binaries
- Install vJoy and enable it.
- Run the MouseSim executable and select a config (1- Racing, 2- Truck, 3- Formula)
  - Which can be configured after they are created.
- Go for a gap that don't exist.

# Building
## Requirements
- Windows
- C++ Compiler
- [xmake](https://xmake.io)
## Steps
- `xmake f -v --arch=x86 -m release`
- `xmake`
- `xmake package` -> Puts built binaries to `./packaged`