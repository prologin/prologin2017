#!/usr/bin/python

import django
import os
import sys
import time

from django.contrib.auth.models import User
from prologin.concours.stechec.models import Tournament, Match, MatchPlayer, Champion, TournamentPlayer, Map

from django.utils.dateparse import parse_datetime
import pytz
date_debut = parse_datetime('2017-05-01 00:57:00')
date_debut = pytz.timezone('Europe/Paris').localize(date_debut, is_dst=None)

def get_champions():
    chs = []
    for u in User.objects.all():
        ch = Champion.objects.filter(author=u, author__is_staff=False, status='ready',
                                     deleted=False, ts__lte=date_debut).order_by('-id')
        if len(ch) > 0:
            chs.append(ch[0])
    return chs
