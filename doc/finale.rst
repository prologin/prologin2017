======
Manuel
======

------
Établi
------

Chaque établi peut être représenté par une grille carrée de 6 cases de côté.
Chaque apprenti a son propre établi.

Élément
=======

Les éléments sont les matières premières mis à votre disposition. Il y
en a cinq :

* Le *plomb*, le *fer* et le *cuivre* sont les trois métaux que vous pouvez
  transmuter en or. Les trois ont le même rendement, mais ne peuvent
  pas être mélangés.
* Le *mercure* et le *soufre* sont les deux éléments volatiles qui permettront
  de catalyser un élément vers un autre élément.

Échantillon
===========

Un échantillon est une paire d'éléments. Ils peuvent être composés de deux
éléments identiques ou distincts.


---------------------
Déroulement d'un tour
---------------------

Au début de votre tour, vous recevrez l'échantillon créé par votre
adversaire pour vous au tour précédent. Vous pouvez réaliser les actions
ci-dessous dans n'importe quel ordre.

Actions
=======

Donner un échantillon
---------------------

Pour inculper la solidarité et exacerber l'entraîde entre ses apprentis, il est
demandé à tous les apprentis de se rendre service et de ne jamais se servir
soit-même dans la réserve. (FIXME : footnote "il ne vous viendrait pas à
l'esprit de lui désobéir))

À chaque tour N, les deux apprentis créent un échantillon pour leur adversaire,
échantillon qui sera posé au tour N+1. Si vous oubliez de créer cet
échantillon, votre adversaire recevra un échantillon identique à celui qu'il a
lui-même créé au tour précédent.
Par souci d'égalité, chaque échantillon doit avoir au minimum *un élément en
commun* avec l'échantillon reçu au tour précédent. Par exemple, si l'apprenti A
confie un échantillon de plomb et de mercure à l'apprenti B, l'apprenti B devra
lui rendre un échantillon contenant un élément de son choix, et soit du
mercure, soit du plomb.

Poser un échantillon
--------------------

Une fois l'échantillon en main, vous devez délicatement le déposer sur votre
établi.

* Un élément au moins de l'échantillon doit être posé à un emplacement adjacent
  à un élément de même type.
* Si il n'y a aucun élément identique entre l'échantillon et ce qui est déjà
  posé sur l'établi, l'échantillon peut être posé n'importe où.
* Si il n'y a pas d'emplacement valide, vous ne pouvez pas poser l'échantillon
  et il ne vous reste plus qu'à le jeter et assassiner discrètement votre compère
  qui - n'en doutez pas ! - savait pertinemment quel tour il était en train de
  vous jouer.

FIXME exemples

Transmuter
----------

Lorsque vous possédez une zone d'un certain métal (cuivre, fer ou plomb), vous
pouvez lancer la *transmutation* en or. Plus cette zone est grande, plus vous
obtiendrez d'or.

// FIXME Insérez ici la formule.

Vous pouvez transmuter un unique bloc de métal. Néanmoins, cela revient à
détruire ce bout d'élément et donc à perdre de l'or - stratégie aussi
audacieuse que dangereuse.

Catalyser
---------

Lorsque vous possédez une zone suffisamment large de catalyseur (mercure ou
soufre), vous pouvez sublimer ces éléments, les faisant ainsi disparaître de
l'établi. Le catalyseur obtenu vous permettra de transformer un élément
quelconque de l'établi en un autre élément de votre choix.
Rien ne vous empêche de catalyser un élément de l'établi d'un autre apprenti,
tant que celui-ci a le dos tourné...

Score
=====

Votre score est déterminé par la quantité d'or que vous serez parvenu à
transmuter en ``NB_TOURS``.
