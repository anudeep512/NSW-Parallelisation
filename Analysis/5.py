import matplotlib.pyplot as plt



# X coordinates
x = [8, 16, 32, 128, 256]

# Two sets of Y coordinates
y1 = [1.19312,
1.19603,
1.19515,
1.2109,
1.20407]
y2 = [0.698906,
0.609947,
0.57873,
0.621435,
0.773324]
y3 = [0.721993,
0.590038,
0.585586,
0.651357,
0.766042,
]
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
    