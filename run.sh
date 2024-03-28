#!/bin/bash
scripts_dir=$(realpath $(dirname $0))
output_dir=$1

generate_command=(python $scripts_dir/python/generate.py)
render_command=(python $scripts_dir/render_commits/render.py)

source GIT_VARS

# Setup repo
mkdir $output_dir
cp template/* $output_dir

cd $output_dir

git init
git commit -m "Initial commit"

$generate_command --out .git/commits.bin --num 1
$render_command --in .git/commits.bin
