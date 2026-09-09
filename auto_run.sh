#! /bin/bash

MODULE_NAME="mye1000e"

echo "=== 1. trying to remove module ==="
sudo rmmod $MODULE_NAME 2>/dev/null

if [ $? -eq 0 ]; then
    echo "Successfully removed $MODULE_NAME"
else
    echo "Module $MODULE_NAME is not loaded or failed to remove"
fi

echo "=== 3. Reload Module ==="
sudo insmod ${MODULE_NAME}.ko

if [ $? -eq 0 ]; then
    echo "Successfully loaded ${MODULE_NAME}.ko!"
    echo "=== 4. View latest 10 lines of dmesg ==="
    dmesg | tail -n 10
else
    echo "Failed to load ${MODULE_NAME}.ko, please check dmesg!"
fi