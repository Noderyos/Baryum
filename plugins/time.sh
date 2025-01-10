#!/bin/bash

source plugins/colors.sh

custom_time() {
	WEEKDAY=$(date +%A)
	TIME=$(date +%H:%M:%S)
	DATE=$(date +"%e %b %Y")

	echo "$light0_soft {B}$WEEKDAY $faded_yellow $TIME $light0_soft $DATE {B-}$RESET"
}

custom_time
