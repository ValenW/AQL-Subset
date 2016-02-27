每个.aql文件对应一个同名的文件夹，内有适用于该aql程序的若干文章，以.input或者.txt为后缀名。.output文件为运行程序后的输出内容。

perloc是一个简单的数据集，可以用来简单的测试。

revenue则较复杂，全面使用了AQL Subset的功能。
其中创建的两个view：一个RevenueAndDivision，另一个RevenueAndDivision2，实际上应该合成一个view的，但是因为本子集的功能有限，针对不同的pattern但同一种提取信息，只好分开创建view并提取。

revenueP在revenue的基础上增加了难度，用于性能测试。
其中有以Token{1, 20}开头、结尾的的RevenueAndDivision和RevenueAndDivision2，以及开头结尾都为Token{1, 20}的RevenueAndDivision3

China.aql是对中国的朝代及时间进行分析，文本是wiki上的China和China history条目。

Maldives.aql是对马来西亚的政党、总统及相关时间进行分析，文本来源于wiki上的Maldives条目。

Mckinsey.aql是对数据科学(Data Science)相关的文章进行分析，两篇文章分别来自麦肯锡网站和readwrite.com上的博文。
dataScient：
http://www.mckinsey.com/insights/business_technology/big_data_the_next_frontier_for_innovation
salary：
http://readwrite.com/2014/07/21/data-scientist-income-skills-jobs

