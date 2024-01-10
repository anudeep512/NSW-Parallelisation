import matplotlib.pyplot as plt



# X coordinates
x = [8, 16, 32, 128, 256]

# Two sets of Y coordinates
y1 = [18.01,
17.95,
17.98,
 18.09,
 18.17]
y2 = [8.10,
7.78,
8.13,
 9.56,
 12.75]
y3 = [8.36,
7.90,
8.26,
 9.70,
 12.88]
# Plotting the graph
plt.plot(x, y1, marker='o',label='Sequential')
plt.plot(x, y2,marker='o', label='SAM Parallel')
plt.plot(x, y3,marker='o',   label='DAM Parallel')
# Adding labels and title
plt.xlabel('Number of entries')
plt.ylabel('Time of execution (in sec)')
plt.title('Time of execution (vs) Number of entries')

textstr = '\n'.join((
    r'Number of Threads: $16$',
    r'K: $5$',
    r'Dataset size: $5000$'))
props = dict(boxstyle='round', facecolor='wheat', alpha=0.8)
plt.text(0.65, 0.90, textstr, transform=plt.gca().transAxes, fontsize=9,
         verticalalignment='top', bbox=props)

# Adding legend
plt.legend()

# Show the plot
plt.show()
    