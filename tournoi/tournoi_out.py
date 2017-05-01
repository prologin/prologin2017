#!/usr/bin/python

import django
import os
import sys
import time
from collections import defaultdict

if len(sys.argv) < 2:
    print('Usage: {} tournament_id'.format(sys.argv[0]))
    sys.exit(1)

os.environ['DJANGO_SETTINGS_MODULE'] = 'prologin.concours.settings'
django.setup()

from django.contrib.auth.models import User
from prologin.concours.stechec.models import Tournament, Match, MatchPlayer, Champion, TournamentPlayer, Map

from tournoi_common import get_champions

chs = get_champions()

tid = int(sys.argv[1])

tournoi = Tournament.objects.get(id=tid)
matches = Match.objects.filter(tournament=tournoi)

score = defaultdict(int)
indice = defaultdict(int)
matrix = defaultdict(list)
match_ids = defaultdict(list)
for m in matches:
    c1, c2 = tuple(MatchPlayer.objects.filter(match=m))
    matrix[(c1.champion.id, c2.champion.id)].append(c1.score)
    matrix[(c2.champion.id, c1.champion.id)].append(c2.score)

    match_ids[(c1.champion.id, c2.champion.id)].append(m.id)
    match_ids[(c2.champion.id, c1.champion.id)].append(m.id)

    # Indices
    indice[c1.champion.id] += c1.score
    indice[c2.champion.id] += c2.score

    # Victories
    if c1.score > c2.score:
        score[c1.champion.id] += 2
    elif c2.score > c1.score:
        score[c2.champion.id] += 2
    else:
        score[c1.champion.id] += 1
        score[c2.champion.id] += 1

print('<!DOCTYPE html><head><title>Prologin 2017 : {}</title></head><body>'.format(tournoi.name))

print('''<style>
table, th, td {
    border: 1px solid black;
    border-collapse: collapse
}

th, td {
    padding-left: 10px;
    padding-right: 10px;
}

.won {
    color: green;
}

td > a {
    color: inherit;
}
.lost {
    color: white;
    background-color: grey;
}

table { border-collapse: collapse; text-align: left; width: 100%; } {font: normal 12px/150% Arial, Helvetica, sans-serif; background: #fff; overflow: hidden; border: 1px solid #8C8C8C; -webkit-border-radius: 3px; -moz-border-radius: 3px; border-radius: 3px; }table td, table th { padding: 3px 10px; }table thead td {background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #8C8C8C), color-stop(1, #7D7D7D) );background:-moz-linear-gradient( center top, #8C8C8C 5%, #7D7D7D 100% );filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#8C8C8C', endColorstr='#7D7D7D');background-color:#8C8C8C; color:#FFFFFF; font-size: 15px; font-weight: bold; border-left: 1px solid #A3A3A3; } table thead td:first-child { border: none; }table tbody td { color: #7D7D7D; border-left: 1px solid #DBDBDB;font-size: 12px;font-weight: normal; }table tbody .alt td { background: #EBEBEB; color: #7D7D7D; }table tbody td:first-child { border-left: none; }table tbody tr:last-child td { border-bottom: none; }table tfoot td div { border-top: 1px solid #8C8C8C;background: #EBEBEB;} table tfoot td { padding: 0; font-size: 12px } table tfoot td div{ padding: 2px; }table tfoot td ul { margin: 0; padding:0; list-style: none; text-align: right; }table tfoot  li { display: inline; }table tfoot li a { text-decoration: none; display: inline-block;  padding: 2px 8px; margin: 1px;color: #F5F5F5;border: 1px solid #8C8C8C;-webkit-border-radius: 3px; -moz-border-radius: 3px; border-radius: 3px; background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #8C8C8C), color-stop(1, #7D7D7D) );background:-moz-linear-gradient( center top, #8C8C8C 5%, #7D7D7D 100% );filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#8C8C8C', endColorstr='#7D7D7D');background-color:#8C8C8C; }table tfoot ul.active, table tfoot ul a:hover { text-decoration: none;border-color: #7D7D7D; color: #F5F5F5; background: none; background-color:#8C8C8C;}div.dhtmlx_window_active, div.dhx_modal_cover_dv { position: fixed !important; }
</style>''')

print('<h1>{}</h1>'.format(tournoi.name))
print('<h2>Classement</h2>')

print('<table>')
print('<thead><td>#</td><td>Victoires</td><td>Nom</td><td>Login</td><td>Nom</td></thead>')
l = chs[:]
l.sort(key=lambda x: -score.get(x.pk, 0))
for i, c in enumerate(l, 1):
    print('<tr>')
    print('<td>{}</td>'.format(i))
    print('<td>{}</td>'.format(score.get(c.pk, 0) // 2))
    print('<td><strong><a href="http://concours/champions/{}/sources/">{}</a></strong></td>'.format(c.id, c.name))
    print('<td>{}</td><td>{}</td>'.format(c.author.username, c.author.get_full_name()))
    print('</tr>')
print('</table>')

for i, c1 in enumerate(l, 1):
    print('<h2>{}. {} (de {})</h2>'.format(i, c1.name, c1.author.username))

    print('<table>')
    print('<thead>')
    print('<td>VS</td>')
    for i in range(len(matrix[c1.pk, l[0].pk])):
        print('<td>Match {}</td>'.format(i + 1))
    print('<td>Total</td>')
    print('</thead>')

    for i, c2 in enumerate(l, 1):
        print('<tr><td><strong>{}</strong>'.format(i))

        for j in range(len(matrix[c1.pk, c2.pk])):
            if c1.pk == c2.pk:
                print('<td>X</td>')
            else:
                color = 'red' if matrix[c1.pk, c2.pk][j] < matrix[c2.pk, c1.pk][j] else 'green'
                print('<td><a style="color: {}" href="http://concours/matches/{}">{}</a></td>'.format(color, match_ids[c1.pk, c2.pk][j], matrix[c1.pk, c2.pk][j]))
        print('<td><strong>{}</strong></td>'.format(sum(matrix[c1.pk, c2.pk])))
        print('</tr>')
    print('</table>')

print('</body></html>')
