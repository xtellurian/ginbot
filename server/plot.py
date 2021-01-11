import matplotlib.pyplot as plt
import matplotlib.animation as animation
import seaborn as sns
import pandas as pd

# set this for the run
path='run1.csv'



# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

sns.set_palette("magma")

def load_data():
    print(f'Reading {path}')
    df = pd.read_csv(path, names=['Timestamp', 'Id', 'Value'])
    df['Timestamp'] = pd.to_datetime(df.Timestamp)
    df['seconds'] = (df.Timestamp - df.Timestamp.min()).dt.total_seconds()
    print('Reloaded DataFrame...')
    return df

def process_data(df):
    df = df.pivot(index='seconds', columns='Id', values='Value')
    rolling_mean = df.rolling(20,  min_periods=1).mean()
    df['s1_roll'] = rolling_mean['s1']
    df['s2_roll'] = rolling_mean['s2']
    return df

# This function is called periodically from FuncAnimation
def animate(i):
    df = load_data()
    df = process_data(df)
    print(df.tail(1))

    ax.clear()
    # g = sns.lineplot(data=df, x=df.index, y=df.s1, ax=ax, label="S1")
    # g = sns.lineplot(data=df, x=df.index, y=df.s2, ax=ax, label="S2")
    g = sns.lineplot(data=df, x=df.index, y=df.s1_roll, ax=ax, label="S1 Mean")
    g = sns.lineplot(data=df, x=df.index, y=df.s2_roll, ax=ax, label="S2 Mean")
    g.set(title='GinBot Temperatures', ylabel='Degrees C')



# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, interval=5000)
plt.show()
