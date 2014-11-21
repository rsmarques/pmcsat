#!/bin/bash

mkdir -p binary

cd /simp/
make rs
cp pmcsat_static ../binary
 
