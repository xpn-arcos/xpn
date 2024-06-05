#!/bin/bash
set -x

hostname=$(cat /etc/hostname)
sudo docker ps --format '{{.ID}} {{.Names}}' | grep -v $hostname | awk '{print $1}' | head -1 | xargs sudo docker kill