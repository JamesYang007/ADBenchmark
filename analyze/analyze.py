import os
import matplotlib.pyplot as plt
import pandas as pd

figpath = '../docs/figs'
csvpath = '../build/benchmark'
csvfiles = [f for f in os.listdir(csvpath)
                if os.path.isfile(os.path.join(csvpath, f)) and
                   f.endswith('_eval.csv')]
plt.rcParams["font.size"] = "12"

def analyze(path, name):
    print(path)
    df = pd.read_csv(path)
    df.set_index('N', inplace=True)
    fastad_col = df['fastad']
    df = df.apply(lambda col: col / fastad_col)
    df.reset_index(level=df.index.names, inplace=True)
    axes = df.plot(x='N',
                   kind='line',
                   marker='.',
                   xticks=df['N'],
                   xlabel='N (input size)',
                   ylabel='Avg Time / FastAD Avg Time',
                   title=name,
                   figsize=(8,6))
    axes.set_xscale('log', base=2)
    axes.set_yscale('log', base=2)
    plt.savefig(os.path.join(figpath, name + '_fig.png'))

for f in csvfiles:
    analyze(os.path.join(csvpath, f), f.replace('_eval.csv', ''))
