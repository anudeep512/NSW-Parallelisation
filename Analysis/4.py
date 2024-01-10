import matplotlib.pyplot as plt

# X coordinates
x = [1, 2, 4, 8, 16]

# Two sets of Y coordinates

y1 = [ 0,0.4,1.4,4.4,3.6 ]
y2 = [0,238.6,630.2,702.4,694.8]
y3 = [0,0,0,0,0]

# Plotting the graph
plt.plot(x, y2, marker='o', label='$K = 1$')
plt.plot(x, y1,  marker='o', label='$K = 2$')
plt.plot(x,y3,  marker='o', label = '$K = 3$')
# Adding labels and title
plt.xlabel('Number of threads')
plt.ylabel('Number of Error Neighbors')
plt.title('')

textstr = '\n'.join((
    r'Number of Entries: $8$',
    r'Dataset size: $5000$',
    r'Algorithm used: DAM'))
props = dict(boxstyle='round', facecolor='wheat', alpha=0.8)
plt.text(0.65, 0.96, textstr, transform=plt.gca().transAxes, fontsize=9,
         verticalalignment='top', bbox=props)

# Adding legend
plt.legend()

# Adding legend
plt.legend()

# Show the plot
plt.show()









