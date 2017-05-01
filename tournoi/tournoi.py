#!/usr/bin/python

import collections
import django
import os
import sys
import time

os.environ['DJANGO_SETTINGS_MODULE'] = 'prologin.concours.settings'
django.setup()

from django.db import connection
from django.contrib.auth.models import User
from django.utils.timezone import now
from prologin.concours.stechec.models import Tournament, Match, MatchPlayer, Champion, TournamentPlayer, Map

from tournoi_common import get_champions

prologin = User.objects.get(username="seirl")
tournoi = Tournament.objects.create()

print('Launching tournament {}'.format(tournoi.id))

chs = get_champions()

print()
print('Champions are :')
for ch in chs:
    print('-', ch)

input()
print()
print('Launching matches...')
match_objs = []
ts = now()
for i in range(len(chs) ** 2):
    for i in range(10):
        m = Match(author=prologin, tournament=tournoi)
        m.ts = ts
        match_objs.append(m)
Match.objects.bulk_create(match_objs)

matches = Match.objects.filter(ts=ts)
matches_queue = list(matches)

player_objs = []
for c1 in chs:
    for c2 in chs:
        for i in range(10):
            m = matches_queue.pop()
            player_objs.append(MatchPlayer(champion=c1, match=m))
            player_objs.append(MatchPlayer(champion=c2, match=m))
MatchPlayer.objects.bulk_create(player_objs)

with connection.cursor() as c:
    c.execute('''
        UPDATE stechec_match SET status='new' WHERE id IN (%s)
    ''' % ','.join(str(m.id) for m in list(matches)))
