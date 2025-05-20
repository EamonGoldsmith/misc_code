#!/bin/bash

# wait a random amount of time
echo "running script"
sleep $((RANDOM % 10 + 1))

# randomly fail
echo "script failed"
exit 1
