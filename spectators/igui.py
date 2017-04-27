# coding: utf-8

import tkinter as tk
import tkinter.font as tkFont

from api import *

root = tk.Tk()
CASE_SIZE = 40


def case_to_str(case):
    return {
        case_type.VIDE: " ",
        case_type.FER: "F",
        case_type.CUIVRE: "C",
        case_type.MERCURE: "M",
        case_type.PLOMB: "P",
        case_type.SOUFRE: "S",
    }.get(case, "?")


def postion_from_mouse(event):
    x = event.x // CASE_SIZE
    y = event.y // CASE_SIZE
    return (x, y)


class Application(tk.Frame):  # pylint: disable=too-many-ancestors
    def __init__(self, master=None):
        super().__init__(master)
        self.pack()

        size = (TAILLE_ETABLI * CASE_SIZE) * 2 + CASE_SIZE + 2
        self.canvas = tk.Canvas(self, height=size / 2 + CASE_SIZE, width=size)
        self.canvas.pack()
        self.canvas_next = tk.Canvas(self, height=4 * CASE_SIZE, width=size)
        self.canvas_next.pack(side=tk.BOTTOM)

        self.me = {}
        self.other = {}
        self.sample = (case_type.VIDE, case_type.VIDE)
        self.next_sample = (case_type.VIDE, case_type.VIDE)

        self.last_selected = (-1, -1)
        self.placed = False
        self.my_score = 0
        self.other_score = 0
        self.direction = (1, 0)

        self.draw_grid()

        master.bind('n', self.end)
        master.bind('u', self.undo)
        self.canvas.bind("<Button-1>", self.add_sample)
        self.canvas.bind("<Button-3>", self.activate)
        self.canvas.bind('<Motion>', self.motion)
        self.canvas_next.bind("<Button-1>", self.select_next_sample)

    def update(self, pull_api=True):
        if pull_api:
            me = moi()
            other = adversaire()
            for i in range(0, TAILLE_ETABLI):
                for j in range(0, TAILLE_ETABLI):
                    self.me[(i, j)] = type_case((i, j), me)
                    self.other[(i, j)] = type_case((i, j), other)
            self.sample = echantillon_tour()
            self.placed = a_pose_echantillon()
            self.my_score = score(me)
            self.other_score = score(other)
        self.canvas.delete("all")
        self.canvas_next.delete("all")
        self.draw_grid()

    def draw_grid(self):
        helv = tkFont.Font(family='Helvetica', size=int(CASE_SIZE * 0.8), weight='bold')
        if self.last_selected != (-1, -1):
            wb = 0
            x, y = self.last_selected
            offset = 1 if wb == 0 else 1 + (TAILLE_ETABLI + 1) * CASE_SIZE
            self.canvas.create_rectangle(
                offset + x * CASE_SIZE, offset + y * CASE_SIZE,
                offset + (x + 1) * CASE_SIZE, offset + (y + 1) * CASE_SIZE,
                fill="#AAAAAA")
            if not self.placed and placement_possible_echantillon(self.sample, (x, y),
                                                                  (x + self.direction[0], y + self.direction[1]),
                                                                  moi()):
                self.canvas.create_text((offset + (x + 0.5) * CASE_SIZE + 5,
                                         (y + 0.5) * CASE_SIZE + 5), text=case_to_str(self.sample[0]), font=helv,
                                        fill="blue")
                self.canvas.create_text((offset + (x + self.direction[0] + 0.5) * CASE_SIZE + 5,
                                         (y + self.direction[1] + 0.5) * CASE_SIZE + 5),
                                        text=case_to_str(self.sample[1]), font=helv, fill="blue")
        for i in range(0, TAILLE_ETABLI + 1):
            for offset in (1, 1 + (TAILLE_ETABLI + 1) * CASE_SIZE):
                self.canvas.create_line(offset + i * CASE_SIZE,
                                        1 + CASE_SIZE * TAILLE_ETABLI,
                                        offset + i * CASE_SIZE, 1, dash=(4, 2))
                self.canvas.create_line(offset + CASE_SIZE * TAILLE_ETABLI,
                                        1 + i * CASE_SIZE,
                                        offset, 1 + i * CASE_SIZE, dash=(4, 2))
        for d, offset in ((self.me, 0), (self.other, (TAILLE_ETABLI + 1) * CASE_SIZE)):
            for pos, case in d.items():
                if case == case_type.VIDE:
                    continue
                self.canvas.create_text((offset + (pos[0] + 0.5) * CASE_SIZE + 5,
                                         (pos[1] + 0.5) * CASE_SIZE + 5), text=case_to_str(case), font=helv)
        helv16 = tkFont.Font(family='Helvetica', size=16)
        self.canvas.create_text(0, CASE_SIZE * (TAILLE_ETABLI),
                                text="Score : {}".format(self.my_score), font=helv16, anchor="nw")
        self.canvas.create_text((TAILLE_ETABLI + 1) * CASE_SIZE, CASE_SIZE * (TAILLE_ETABLI),
                                text="Score : {}".format(self.other_score), font=helv16, anchor="nw")
        self.canvas.create_text(0, CASE_SIZE * (TAILLE_ETABLI + 0.8),
                                text="Échantillon du tour :", font=helv16, anchor="nw")
        self.canvas.create_text((TAILLE_ETABLI + 1) * CASE_SIZE, CASE_SIZE * (TAILLE_ETABLI + 0.8),
                                text="Prochain échantillon :", font=helv16, anchor="nw")
        color = "#AAAAAA" if self.placed else "black"
        self.canvas_next.create_text(CASE_SIZE, CASE_SIZE,
                                     text=case_to_str(self.sample[0]), font=helv, anchor="nw", fill=color)
        self.canvas_next.create_text((1 + self.direction[0]) * CASE_SIZE, (1 + self.direction[1]) * CASE_SIZE,
                                     text=case_to_str(self.sample[1]), font=helv, anchor="nw", fill=color)
        color = "black"
        if self.next_sample[0] not in self.sample and self.next_sample[1] not in self.sample:
            color = "red"
        self.canvas_next.create_text((TAILLE_ETABLI + 1) * CASE_SIZE, 0,
                                     text=case_to_str(self.next_sample[0]), font=helv, anchor="nw", fill=color)
        self.canvas_next.create_text((TAILLE_ETABLI + 2) * CASE_SIZE, 0,
                                     text=case_to_str(self.next_sample[1]), font=helv, anchor="nw", fill=color)

    def end(self, event):
        donner_echantillon(self.next_sample)
        self.last_selected = (-1, -1)
        root.quit()

    def undo(self, event):
        annuler()
        self.update()

    def add_sample(self, event):
        x, y = postion_from_mouse(event)
        placer_echantillon((x, y), (x + self.direction[0], y + self.direction[1]))
        self.update()

    def activate(self, event):
        x, y = postion_from_mouse(event)
        transmuter((x, y))
        self.update()

    def motion(self, event):
        selected = postion_from_mouse(event)
        x, y = selected
        if not (x >= 0 and y >= 0 and x < TAILLE_ETABLI and y < TAILLE_ETABLI):
            selected = (-1, -1)
        if selected != self.last_selected:
            self.last_selected = selected
            self.update(False)

    def select_next_sample(self, event):
        x = event.x // CASE_SIZE
        if x < 3:
            if self.direction[0] == -1:
                self.direction = (0, -1)
            elif self.direction[1] == -1:
                self.direction = (1, 0)
            elif self.direction[0] == 1:
                self.direction = (0, 1)
            else:
                self.direction = (-1, 0)
            self.update(False)
        elif TAILLE_ETABLI + 1 <= x < TAILLE_ETABLI + 3:
            x -= TAILLE_ETABLI + 1
            l = list(case_type)
            v = l[(l.index(self.next_sample[x]) % (len(l) - 1)) + 1]
            if x == 0:
                self.next_sample = (v, self.next_sample[1])
            else:
                self.next_sample = (self.next_sample[0], v)
            self.update(False)


app = Application(master=root)


# Fonction appelée au début de la partie.
def partie_init():
    # Place ton code ici
    pass


# Fonction appelée à chaque tour.
def jouer_tour():
    global app
    app.next_sample = echantillon_tour()
    app.update()
    app.mainloop()


# Fonction appelée à la fin de la partie.
def partie_fin():
    # Place ton code ici
    pass
