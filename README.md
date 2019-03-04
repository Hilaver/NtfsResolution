# NtfsResolution

Author：老张

这是一个NTFS文件系统目录树的遍历Demo，跳过了活动分区。

代码原理在 https://blog.csdn.net/hilavergil/article/details/79270379

目前仅支持MBR分区模式的磁盘(这部分代码建议在虚拟机上运行)

首先，代码读取磁盘的MBR扇区，并解析MBR引导代码后面的分区表，获取主分区和扩展分区(如果存在)的物理偏移。

其次，如果扩展分区存在，则进一步定位其中包含的逻辑分区。

分区定位完成后，判断分区是否为NTFS(仅学了NTFS文件系统)格式，若是则读取该分区的DBR扇区并定位MFT。

最后，从MFT中找到根目录项，即NTFS目录树(B+树)的根节点，深度优先遍历(大致是 文件记录项-->索引项-->文件记录项-->...-->文件记录项 的递归)，定位目录的子目录项，定位文件的0X80属性(数据属性)，直到所有节点遍历完成。
