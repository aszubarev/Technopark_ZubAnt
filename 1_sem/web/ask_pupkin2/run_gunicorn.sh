#!/bin/bash

cd /home/vadim/ZubAnt/TechnoPark/1_sem/web/ask_pupkin2

exec gunicorn -c /home/vadim/ZubAnt/TechnoPark/1_sem/web/ask_pupkin2/gunicorn.conf.py ask_pupkin2.wsgi:application

