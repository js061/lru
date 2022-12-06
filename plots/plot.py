#!/opt/homebrew/bin/python3
import matplotlib.pyplot as plt

mem_size = [65, 70, 72, 75, 77, 80, 85, 90, 92, 95, 100, 110, 120, 130, 140]
percentage = [1.0, 1.0, 0.797, 0.797, 0.797, 0.397, 0.397, 0.397, 0.397, 0, 0, 0.182, 0.434, 0.688, 0.94]
percentage_lru = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]
percentage_solution = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]

plt.plot(mem_size, percentage_lru, marker='o', linestyle='dashed', label='lru')
plt.plot(mem_size, percentage_solution, marker='x', linestyle='dashed', label='lru_tier_from_refs, refs == 0 ? 0 : refs + 1')
plt.plot(mem_size, percentage, marker='o', label='lru_tier_from_refs, log2(refs + 1)')
plt.plot(mem_size, percentage, marker='x', linestyle='dashed', label='lru_tier_from_refs, (refs + 1)/2')

#plt.legend()
plt.xlabel('memory limit (MB)')
plt.ylabel('percentage of repeatedly requested file in page cache')
plt.xticks(mem_size)
#plt.xscale('log', base=2)
# plt.yscale('log')

plt.legend(loc='upper center', borderpad=0.1, labelspacing=0.1, bbox_to_anchor=(0.55, 0.95))

plt.savefig('plot.png')
plt.clf()

times = [4, 8, 12, 16]
percentage_80 = [0, 0.397, 0.397, 0.397]
percentage_65 = [0, 1.0, 1.0, 1.0]
percentage_75 = [0, 0.797, 0.797, 0.797]
percentage_110 = [0.182, 0.182, 0.182, 0.182]
percentage_130 = [0.688, 0.688, 0.688, 0.688]

plt.plot(times, percentage_80, marker='o', label='80MB')
plt.plot(times, percentage_65, marker='o', label='65MB')
plt.plot(times, percentage_75, marker='o', label='75MB')
plt.plot(times, percentage_110, marker='o', label='110MB')
plt.plot(times, percentage_130, marker='o', label='130MB')
plt.xlabel('Access times (read)')
plt.ylabel('percentage of repeatedly requested file in page cache')
plt.xticks(times)

plt.legend()

plt.savefig('plot_1.png')
plt.clf()

plt.plot(mem_size, percentage, marker='o', label='lru_tier_from_refs, refs == 0 ? 0 : refs + 1')

plt.xlabel('memory limit (MB)')
plt.ylabel('percentage of repeatedly requested file in page cache')
plt.xticks(mem_size)

plt.legend()

plt.savefig('plot_2.png')
plt.clf()