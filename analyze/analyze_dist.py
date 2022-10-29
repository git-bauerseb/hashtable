import matplotlib.pyplot as plt

hist_file = open('./dist', 'r')
content = hist_file.read().replace('[', '').replace(']', '')
nums = content.split(',')
y = [int(x) for x in nums]
x = [i for i in range(len(nums))]

plt.plot(x,y)
plt.show()