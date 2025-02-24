#!/bin/bash
mkdir plugins
make -C src/plugins/sound
make -C src/plugins/i3
make -C src/current_app
make -C src/plugins/time
make
