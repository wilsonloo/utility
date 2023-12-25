# 队空条件：rear = fornt
# 队满条件：(rear+1)%MaxSize == front(零位置没放元素)
# 元素进队：rear = (rear+1)%MaxSize
# 元素出队：front = (front+1)%MaxSize
class CircleQueue:  # 循环队列
    """若只剩下一个空位置，该循环列表锁定，不能再加，但其优势在于可边删边加（剩两个及以上空位时），避免了假溢出"""
    def __init__(self, MaxSize):
        self.cap = MaxSize
        self.data = [None] * self.cap  # 初始空间
        self.front = 0
        self.rear = 0

    def push(self, e):  # 元素e进队
        assert (self.rear + 1) % self.cap != self.front  # 判断队满
        self.rear = (self.rear + 1) % self.cap
        self.data[self.rear] = e

    def is_empty(self):  # 判断队空
        return self.rear == self.front

    def pop(self):  # 元素出队
        assert not self.is_empty()  # 先判断是否为空
        self.front = (self.front + 1) % self.cap
        return self.data[self.front]

    def gethead(self):  # 获取头元素
        assert not self.is_empty()
        return self.data[(self.front + 1) % self.cap]

    def getsize(self):  # 获取队列长度，在front下标小于rear时，size可以直接用rear-front获取，但是如果边删边加，导致rear小于front，此方法出错
        return (self.rear - self.front + self.cap) % self.cap #该式满足上叙所有情况

    def dump(self):
        q=self.front
        if self.front !=self.rear: #判断队空
            for i in range(self.getsize()):
                q = (q+1)%self.cap #符合两种情况的式子
                print(self.data[q], end=",")
        else:
            return None
    
    def pushk(qu, k, e):
        n = qu.getsize()
        if k < 1 or k > n + 1:  #k必须正常
            return False
        if k <= n:
            for i in range(1, n + 1):  #边删边进
                if i == k:  #插个队，它插完，后面的再边删边进
                    qu.push(e)
                x = qu.pop()
                qu.push(x)
        else: 
            qu.push(e)
        return True

    def popk(qu, k):
        n = qu.getsize()
        assert 1 <= k <= n
        for i in range(1, n + 1):  #和上面的思想一样
            x = qu.pop()
            if i != k:
                qu.push(x)
            else:
                e = x  # 取第k个出队的元素
        return e
 
if __name__=="__main__":
    hh = CircleQueue(5)
    print(hh.is_empty())
    hh.push(0)
    hh.push(1)
    hh.push(2)
    hh.push(3)
    print(hh.getsize())
    hh.dump()
# True
# 4
# 0, 1, 2, 3,
# Process
# finished
# with exit code 0