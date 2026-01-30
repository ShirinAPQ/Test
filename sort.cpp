// 冒泡排序 — 核心实现
#include <vector>
#include <algorithm>

// 冒泡排序函数 - 对整数向量进行升序排序
// 参数: a - 待排序的整数向量（原地排序，直接修改原向量）
// 特性: 包含提前退出优化，当数组已排序时可快速终止
void bubble_sort(std::vector<int>& a) {
	size_t n = a.size();  // 获取向量长度
	if (n < 2) return;    // 边界检查：如果元素少于2个，直接返回（已排序）
	
	// 外层循环：控制排序轮次，最多执行n-1轮
	for (size_t i = 0; i < n - 1; ++i) {
		bool swapped = false;  // 标记本轮是否发生交换
		
		// 内层循环：从索引1开始比较相邻元素
		// n-i: 每轮排序后，最大的i个元素已就位，无需再比较
		for (size_t j = 1; j < n - i; ++j) {
			// 如果前一个元素大于当前元素，交换它们的位置
			if (a[j - 1] > a[j]) {
				std::swap(a[j - 1], a[j]);  // 交换相邻元素
				swapped = true;  // 设置交换标志为true
			}
		}
		
		// 提前退出优化：如果本轮没有发生任何交换，说明数组已经有序
		if (!swapped) break; // 已经有序，提前退出
	}
}

// 示例演示：编译此文件后运行将打印排序前后的数组
#include <iostream>

int main() {
	std::vector<int> a = {5, 1, 4, 2, 8, 3};
	std::cout << "Before: ";
	for (int v : a) std::cout << v << ' ';
	std::cout << '\n';

	bubble_sort(a);

	std::cout << "After:  ";
	for (int v : a) std::cout << v << ' ';
	std::cout << '\n';
	return 0;
}