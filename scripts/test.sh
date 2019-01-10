#/bin/bash

# valgrind --leak-check=full --show-leak-kinds=all ./run

valgrind --tool=memcheck --leak-check=full --trace-children=yes --show-leak-kinds=all --leak-check=yes  --log-file=output.log -q ./run

# 12/09 11:54
# ==18296== LEAK SUMMARY:
# ==18296==    definitely lost: 15,072 bytes in 157 blocks
# ==18296==    indirectly lost: 41,775,000 bytes in 313 blocks
# ==18296==      possibly lost: 2,546,500 bytes in 7 blocks
# ==18296==    still reachable: 196,782 bytes in 552 blocks
# ==18296==         suppressed: 0 bytes in 0 blocks
# ==18296==


# 1）使用未初始化的内存

# 2）读/写已经被释放的内存

# 3）读/写内存越界

# 4）读/写不恰当的内存栈空间

# 5）内存泄漏

# 6）使用malloc/new/new[]和free/delete/delete[]不匹配。

# 7）src和dst的重叠