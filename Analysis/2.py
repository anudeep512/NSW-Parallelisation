import matplotlib.pyplot as plt

# X coordinates
x = [2, 4, 8, 16, 32]

# Two sets of Y coordinates
y1 = [13.13,
9.93,
9.48,
 9.59,
 9.75]
y2 = [12.92,
9.72,
9.40,
 9.63,
 9.93]
# Plotting the graph
plt.plot(x, y1, marker='o',label='SAM Parallel')
plt.plot(x, y2,marker='o', label='DAM Parallel')
# Adding labels and title
plt.xlabel('Number of Threads')
plt.ylabel('Time of execution (in sec)')
plt.title('Time of execution (vs) Number of threads')

textstr = '\n'.join((
    r'Number of Entries: $128$',
    r'K: $5$',
    r'Dataset size: $5000$'))
props = dict(boxstyle='round', facecolor='wheat', alpha=0.8)
plt.text(0.66, 0.6, textstr, transform=plt.gca().transAxes, fontsize=9,
         verticalalignment='top', bbox=props)


# Adding legend
plt.legend()

# Show the plot
plt.show()
