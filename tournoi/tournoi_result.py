#!/usr/bin/python

import django
import os
import sys
import time

if len(sys.argv) < 2:
    print('Usage: {} tournament_id'.format(sys.argv[0]))
    sys.exit(1)

os.environ['DJANGO_SETTINGS_MODULE'] = 'prologin.concours.settings'
django.setup()

from django.contrib.auth.models import User
from prologin.concours.stechec.models import Tournament, Match, MatchPlayer, Champion, TournamentPlayer, Map

from tournoi_common import get_champions

chs = get_champions()

tournoi = Tournament.objects.get(id=int(sys.argv[1]))


while True:
    from django.db.models import Count
    done = Match.objects.filter(tournament=tournoi, status='done').count()
    total = Match.objects.filter(tournament=tournoi).count()

    if done < total:
        print('WARNING: This is a temporary result, some matches are not over yet.')
        print('Matchs done / launched: {} / {}'.format(done, total))
    else:
        break
    import time
    time.sleep(1)

input()
print('lol')

#matches = list(Match.objects.filter(createur = prologin, id__gt = 37911))
matches = tournoi.matches.all()

score = {}
indice = {}
for m in matches:
    c1, c2 = tuple(MatchPlayer.objects.filter(match=m))
    indice[c1.champion.id] = indice.get(c1.champion.id, 0) + c1.score
    indice[c2.champion.id] = indice.get(c2.champion.id, 0) + c2.score
    if c1.score > c2.score:
        score[c1.champion.id] = score.get(c1.champion.id, 0) + 2
    elif c2.score > c1.score:
        score[c2.champion.id] = score.get(c2.champion.id, 0) + 2
    else:
        score[c1.champion.id] = score.get(c1.champion.id, 0) + 1
        score[c2.champion.id] = score.get(c2.champion.id, 0) + 1

l = [(ch.author.username, score.get(ch.id, 0), indice.get(ch.id, 0), ch.id) for ch in chs]
l.sort(key=lambda x: x[2])
for e in l:
    print(e)

#print('Saving...')
#for (score, indice, id) in l:
#    ch = Champion.objects.get(pk=id)
#    p = TournamentPlayer(
#        champion = ch,
#        tournament = tournoi,
#        score = score
#    )
#    p.save()
