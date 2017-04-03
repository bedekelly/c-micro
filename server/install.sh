#!/bin/bash


cd c-micro/server/
python3 -m venv venv
source venv/bin/activate
python -m pip install -r requirements.txt
cd ../..
