#! /usr/bin/env python

def options(opt):
    pass

def configure(cfg):
    pass

def build(bld):
    bld.shlib(
        source = '''
            src/action_catalyser.cc
            src/action_donner_echantillon.cc
            src/action_placer_echantillon.cc
            src/action_transmuter.cc
            src/api.cc
            src/entry.cc
            src/game_state.cc
            src/interface.cc
            src/rules.cc
        ''',
        defines = ['MODULE_COLOR=ANSI_COL_BROWN', 'MODULE_NAME="rules"'],
        target = 'prologin2017',
        use = ['stechec2'],
    )

    bld.install_files('${PREFIX}/share/stechec2/prologin2017', [
        'prologin2017.yml',
    ])
