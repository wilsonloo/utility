export type CircleQueueForeachFunc = (e: any) => void;

export default class CircleQueue<V> {
    get [Symbol.toStringTag]() {
        return 'CircleQueue'
    }

    [Symbol.toPrimitive](hint: string): unknown {
        if (hint === 'string') 
            return `CircleQueue: [ ${this._queue} ]`;
        else 
            throw new Error('CircleQueue不支持转换为 ' + hint + ' 类型');
    }

    toString() {
        return 'CircleQueue: ' + this._queue.toString()
    }

    private readonly _queue: Array<V | undefined>

    // 队列容量
    private readonly _capacity: number
    get capacity(): number {return this._capacity}

    // 队列中当前元素的数量(队列长度)
    get length(): number {return (this._rear + this._capacity - this._front) % this._capacity}

    private _front = 0
    private _rear = 0

    /**
     * 循环队列
     * @param capacity, 循环队列的长度(容量)
     */
    constructor(private readonly cap: number) {
        this._capacity = cap + 1;
        this._queue = Array(this._capacity);
    }

    // 判断队列是否为空
    isEmpty(): boolean {return this._front === this._rear}

    // 判断队列是否已满
    isFull(): boolean {return (this._rear + 1) % this._capacity === this._front}
    
    // 清空队列
    clear(): void {
        this._queue.fill(undefined)
        this._front = this._rear = 0;
    }

    // 弹出一个队首元素
    popHead(): V | undefined {
        if (this.isEmpty()) {
            return undefined;
        }

        const el = this._queue[this._front];
        this._queue[this._front] = undefined;
        this._front = (this._front + 1) % this._capacity;
        return el;
    }

    /**
     * 在队列尾部追加元素
     * @param element   要追加的元素
     * @param autoCover 如果队列满了, 是否自动覆盖队首元素
     */
    push(element: V, autoCover: boolean = true): void {
        if (this.isFull()) {    // 队列满
            if (autoCover) {    // 自动覆盖队头元素
                this._queue[this._front] = undefined;
                this._front = (this._front + 1) % this._capacity;
            } else{
                throw new Error('CircleQueue.push Error: 队列已满,无法继续push');
            }
        }
        this._queue[this._rear] = element;
        this._rear = (this._rear + 1) % this._capacity;
    }

    // 获取队首元素, 如果不存在则返回undefined
    getHead(): V | undefined {
        if (!this.isEmpty())
            return this._queue[this._front];
    }

    // 获取队尾元素, 如果不存在则返回undefined
    getEnd(): V | undefined {
        if (!this.isEmpty())
            return this._queue[(this._rear + this._capacity - 1) % this._capacity];
    }

    // 获取指定索引位置的元素, 如果不存在则返回undefined
    getElement(index: number): V {
        if (index >= this.length) throw new Error('CircleQueue.getElement Error: index索引越界');
        return this._queue[(this._front + index) % this._capacity] as V;
    }

    foreach(func: CircleQueueForeachFunc){
        if(!this.isEmpty()){
            let len = this.length;
            let idx = this._front;
            for(let k = 0; k < len; ++k){
                let e = this._queue[idx];
                idx = (idx + 1) % this._capacity;
                func(e)
            }
        }
    }
}
