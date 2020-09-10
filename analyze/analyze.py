import io
import os
import matplotlib.pyplot as plt
import pandas as pd
from subprocess import check_output

# Path definitions
figpath = '../docs/figs'
libpath = '../build/benchmark'
datapath = '../docs/data'

# List of library names
libs = ['adept', 'adolc', 'baseline', 'cppad', 'fastad', 'sacado', 'stan']

# List of test names
tests = ['log_sum_exp', 'matrix_product', 'normal_log_pdf', 'prod', 'prod_iter',
         'regression', 'stochastic_volatility', 'sum', 'sum_iter']

# Make plot font size bigger
plt.rcParams["font.size"] = "12"

# Creates path to test for lib
def lib_path(libname):
    return os.path.join(libpath, libname)

def bin_name(libname, testname):
    return ''.join([libname, '_', testname])

# Plot result of test
def plot_test(df, name):
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

# Run test for each library
def run(testname):
    df = pd.DataFrame()

    # save current working directory
    cur_path = os.getcwd()

    # run test for each lib and save times
    for lib in libs:

        # change directory to library
        # some libraries may require this to read configuration file
        os.chdir(lib_path(lib))

        path = os.path.join('.', bin_name(lib, testname))
        print(path)

        # run and get output from each
        args = (path, "--benchmark_format=csv")
        data = io.StringIO(check_output(args).decode("utf-8"))
        df_lib = pd.read_csv(data, sep=',')
        df_lib.set_index('N', inplace=True)
        df[lib] = df_lib['real_time']

        # change back to current working directory
        os.chdir(cur_path)

    # save absolute time
    data_path = os.path.join(datapath, testname + ".csv")
    df.to_csv(data_path)

    # create relative time to fastad
    fastad_col = df['fastad'].copy()
    df = df.apply(lambda col: col / fastad_col)
    df.reset_index(level=df.index.names, inplace=True)

    return df

# For each test, run and plot
for test in tests:
    plot_test(run(test), test)
