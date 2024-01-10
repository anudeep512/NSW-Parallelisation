import matplotlib.pyplot as plt

# X coordinates
x = [1000, 2000, 3000, 4000, 5000]

# Two sets of Y coordinates
y1 = [
.83,
2.99,
6.58,
11.50,
18.03]
y2 = [.61,
1.93,
3.90,
6.32,
9.58]
y3 = [.63,
2.00,
3.99,
6.41,
9.60]
# Plotting the graph
plt.plot(x, y1,marker='o', label='Sequential')
plt.plot(x, y2,marker='o', label='SAM Parallel')
plt.plot(x,y3,marker='o', label = 'DAM Parallel')
# Adding labels and title
plt.xlabel('Dataset Size')
plt.ylabel('Time of execution (in sec)')
plt.title('Time of execution (vs) Size of Dataset')

textstr = '\n'.join((
    r'Number of Entries: $128$',
    r'K: $5$',
    r'Number of Threads: $16$'))
props = dict(boxstyle='round', facecolor='wheat', alpha=0.8)
plt.text(0.65, 0.96, textstr, transform=plt.gca().transAxes, fontsize=9,
         verticalalignment='top', bbox=props)


# Adding legend
plt.legend()

# Show the plot
plt.show()
