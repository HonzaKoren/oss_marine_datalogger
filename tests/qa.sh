#!/bin/bash

# Run code quality scanners

find . -iname "*.py" -type f -exec echo {} \; -exec pylint {} \;

pylama --linters pyflakes .
