import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
import seaborn as sns
import numpy as np
import pandas as pd

# set this for the run
path='run1.csv'
directory = "data"


# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

sns.set_palette("magma")

def load_data():
    # print(f'Reading files in {directory}')
    _, _, filenames = next(os.walk(directory))
    df = pd.DataFrame(columns=['Timestamp', 'Id', 'Value'])
    for f in filenames:
        if f[0] == '.':
            continue
        temp_filename = f'{directory}/{f}'
        temp_df = pd.read_csv(temp_filename, names=['Timestamp', 'Id', 'Value'])
        df = df.append(temp_df)
        
    df['Timestamp'] = pd.to_datetime(df.Timestamp)
    df['seconds'] = (df.Timestamp - df.Timestamp.min()).dt.total_seconds()
    df.drop_duplicates(subset=['Timestamp'])
    # print('-------------------------------')
    return df

def process_data(df):
    df_pivot = df.pivot(index='seconds', columns='Id', values='Value')
    rolling_mean = df_pivot.rolling(20,  min_periods=1).mean()
    df_pivot['s1_roll'] = rolling_mean['s1'] if 's1' in rolling_mean.columns else np.nan
    df_pivot['s2_roll'] = rolling_mean['s2'] if 's2' in rolling_mean.columns else np.nan
    return df_pivot

# This function is called periodically from FuncAnimation
def animate(i):
    df = load_data()
    df = process_data(df)
    print(df.tail(20).mean())
    print('---')
    print()
    print('---')

    ax.clear()
    g = sns.lineplot(data=df, x=df.index, y=df.s1_roll, ax=ax, label="S1 Mean")
    g = sns.lineplot(data=df, x=df.index, y=df.s2_roll, ax=ax, label="S2 Mean")
    g.set(title='GinBot Temperatures', ylabel='Degrees C')



# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, interval=5000)
plt.show()
