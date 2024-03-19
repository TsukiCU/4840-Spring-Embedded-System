# /bin/bash

# used for restarting everything/
make qsys-clean
make qsys
make quartus
make rbf
embedded_command_shell.sh
make dtb