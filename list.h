#ifndef _LINK_LIST_H_
#define _LINK_LIST_H_

struct list_head
{
    struct list_head *next, *prev;
};

/*****************************************************************************
 函 数 名  : list_prefetch
 功能描述  : 告诉cpu那些元素有可能马上就要用到，告诉cpu预取一下，这样可以提
             高速度。
 输入参数  : const void *x  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_prefetch(const void *x) {;}

/*****************************************************************************
 函 数 名  : offsetof
 功能描述  : 在已知某一个成员变量的名字和结构体类型的情况下，计算该成员相对于
             结构体的起始地址的偏移量.
 输入参数  : TYPE    :结构体类型名称
             MEMBER  :结构体中成员
 输出参数  : 无
 返 回 值  : 该成员相对于结构体的起始地址的偏移量。
*****************************************************************************/
#define list_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


/*****************************************************************************
 函 数 名  : container_of
 功能描述  : 已知某一个成员变量的名字、指针和结构体类型的情况下，计算结构体的
             指针，也就是计算结构体的起始地址。
 输入参数  : ptr     :某一个成员的指针
             type    :结构体类型
             member  :成员变量名字
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_container_of(ptr, type, member) ({                     \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);        \
        (type *)( (char *)__mptr - list_offsetof(type,member) );})


/*****************************************************************************
 函 数 名  : LIST_HEAD_INIT
 功能描述  : 初始化一个结点名字为name的双向循环链表的头结点
 输入参数  : name  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define LIST_HEAD_INIT(name) { &(name), &(name) }


/*****************************************************************************
 函 数 名  : LIST_HEAD
 功能描述  : 初始化一个结点名字为name的双向循环链表的头结点
 输入参数  : name  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


/*****************************************************************************
 函 数 名  : INIT_LIST_HEAD
 功能描述  : 初始化一个结点名字为name的双向循环链表的头结点
 输入参数  : struct list_head *list  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}


/*****************************************************************************
 函 数 名  : __list_add
 功能描述  : 添加结点new到prev和next之间
 输入参数  : struct list_head *new   
             struct list_head *prev  
             struct list_head *next  
 输出参数  : 无
 返 回 值  : void

*****************************************************************************/
static inline void __list_add(struct list_head *new,
                       struct list_head *prev,
                       struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}


/*****************************************************************************
 函 数 名  : list_add
 功能描述  : 添加结点new到链表头
 输入参数  : struct list_head *new   
             struct list_head *head  
 输出参数  : 无
 返 回 值  : void

*****************************************************************************/
static inline void list_add(struct list_head *new, 
                     struct list_head *head)
{
    __list_add(new, head, head->next);
}


/*****************************************************************************
 函 数 名  : list_add_tail
 功能描述  : 添加结点new到链表尾
 输入参数  : struct list_head *new   
             struct list_head *head  
 输出参数  : 无
 返 回 值  : void

*****************************************************************************/
static inline void list_add_tail(struct list_head *new, 
                          struct list_head *head)
{
    __list_add(new, head->prev, head);
}


/*****************************************************************************
 函 数 名  : __list_del
 功能描述  : 删除结点
 输入参数  : struct list_head *prev:被删除结点的前驱结点
             struct list_head *next:被删除结点后驱结点
 输出参数  : 无
 返 回 值  : void

*****************************************************************************/
static inline void __list_del(struct list_head *prev, 
                       struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}


/*****************************************************************************
 函 数 名  : list_del
 功能描述  : 删除结点entry，将entry的prev指向NULL，next指向NULL.
 输入参数  : struct list_head *entry:被删除结点
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = /*LIST_POISON1*/NULL;
    entry->prev = /*LIST_POISON2*/NULL;
}


/*****************************************************************************
 函 数 名  : list_replace
 功能描述  : 用结点new替换结点old
 输入参数  : struct list_head *old  
             struct list_head *new  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_replace(struct list_head *old,
                         struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

/*****************************************************************************
 函 数 名  : list_replace_init
 功能描述  : 用结点new替换结点old，并初始化old.
 输入参数  : struct list_head *old  
             struct list_head *new  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_replace_init(struct list_head *old,
                              struct list_head *new)
{
    list_replace(old, new);
    INIT_LIST_HEAD(old);
}


/*****************************************************************************
 函 数 名  : list_del_init
 功能描述  : 删除结点entry，并初始化entry.
 输入参数  : struct list_head *entry  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}


/*****************************************************************************
 函 数 名  : list_move
 功能描述  : 先将list节点从原链表中删除，然后将其添加到head链表的表头.
 输入参数  : struct list_head *list  
             struct list_head *head  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_move(struct list_head *list, 
                      struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}


/*****************************************************************************
 函 数 名  : list_move_tail
 功能描述  : 先将list节点从原链表中删除，然后将其添加到head链表的表尾.
 输入参数  : struct list_head *list  
             struct list_head *head  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_move_tail(struct list_head *list,
                           struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}


/*****************************************************************************
 函 数 名  : list_is_last
 功能描述  : 测试list节点是否为head链表的表尾节点。是返回1，否则返回0.
 输入参数  : const struct list_head *list  
             const struct list_head *head  
 输出参数  : 无
 返 回 值  : 1/0
*****************************************************************************/
static inline int list_is_last(const struct list_head *list,
                        const struct list_head *head)
{
    return list->next == head;
}


/*****************************************************************************
 函 数 名  : list_empty
 功能描述  : 判断head链表是否为空链表，是返回1，否则返回为0.
 输入参数  : const struct list_head *head  
 输出参数  : 无
 返 回 值  : 1/0
*****************************************************************************/
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}


/*****************************************************************************
 函 数 名  : list_empty_careful
 功能描述  : 判断节点head的前驱和后驱是否都指向head。是返回1，否则返回0.
 输入参数  : const struct list_head *head  
 输出参数  : 无
 返 回 值  : 1/0
*****************************************************************************/
static inline int list_empty_careful(const struct list_head *head)
{
    struct list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

/*****************************************************************************
 函 数 名  : __list_splice
 功能描述  : 将list链表的全部节点（头节点list除外）插入在prev和next节点之间.
             
 输入参数  : struct list_head *list  
             struct list_head *head  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void __list_splice(struct list_head *list,
                          struct list_head *head)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;
    struct list_head *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}


/*****************************************************************************
 函 数 名  : list_splice
 功能描述  : 在list是非空链表的情况下，将其插在head链表的头部，即head节点的
             后面。此函数不安全，因为在插入后还能通过list节点访问到其余的节
             点.
 输入参数  : struct list_head *list  
             struct list_head *head  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_splice(struct list_head *list, 
                        struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head);
}


/*****************************************************************************
 函 数 名  : list_splice_init
 功能描述  : 在list是非空链表的情况下，将其插在head链表的尾部，即head节点的
             前面。然后对list节点进行初始化，排除不安全因素.
 输入参数  : struct list_head *list  
             struct list_head *head  
 输出参数  : 无
 返 回 值  : void
*****************************************************************************/
static inline void list_splice_init(struct list_head *list,
                             struct list_head *head)
{
    if (!list_empty(list))
    {
        __list_splice(list, head);
        INIT_LIST_HEAD(list);
    }
}

/*****************************************************************************
 函 数 名  : list_entry
 功能描述  : 获取type类型结构体的起始指针
 输入参数  : ptr     :type类型的结构体中member成员的指针
             type    :结构体类型
             member  :结构体中成员
 输出参数  : 无
 返 回 值  : 结构体的起始指针
*****************************************************************************/
#define list_entry(ptr, type, member) \
    list_container_of(ptr, type, member)


/*****************************************************************************
 函 数 名  : list_for_each
 功能描述  : 从head节点开始（不包括head节点！）遍历它的每一个节点.
 输入参数  : pos   :循环指针
             head  :链表头
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each(pos, head)                                        \
    for (pos = (head)->next; list_prefetch(pos->next), pos != (head);   \
            pos = pos->next)

/*****************************************************************************
 函 数 名  : __list_for_each
 功能描述  : 从head节点开始（不包括head节点）遍历它的每一个节点.
 输入参数  : pos   
             head  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define __list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)


/**
 * list_for_each_prev   -   iterate over a list backwards
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */

/*****************************************************************************
 函 数 名  : list_for_each_prev
 功能描述  : 它也是从head节点开始（不包括head节点）向前遍历每一个节点.即从
             链表的尾部开始遍历.
 输入参数  : pos   
             head  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_prev(pos, head)                                   \
    for (pos = (head)->prev; list_prefetch(pos->prev), pos != (head);   \
            pos = pos->prev)


/*****************************************************************************
 函 数 名  : list_for_each_safe
 功能描述  : 从head节点开始（不包括head节点）遍历它的每一个节点.它用n先将
             下一个要遍历的节点保存起来，防止删除本节点后，无法找到下一个节
             点，而出现错误.
 输入参数  : pos   
             n     
             head  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_safe(pos, n, head)                        \
    for (pos = (head)->next, n = pos->next; pos != (head);      \
        pos = n, n = pos->next)


/*****************************************************************************
 函 数 名  : list_for_each_entry
 功能描述  : 已知指向某个结构体的指针pos，以及指向它中member成员的指针head，
             从下一个结构体开始向后遍历这个结构体链。
 输入参数  : pos     
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_entry(pos, head, member)                      \
    for (pos = list_entry((head)->next, typeof(*pos), member);      \
         list_prefetch(pos->member.next), &pos->member != (head);   \
         pos = list_entry(pos->member.next, typeof(*pos), member))


/*****************************************************************************
 函 数 名  : list_for_each_entry_reverse
 功能描述  : 已知指向某个结构体的指针pos，以及指向它中member成员的指针head，
             从下一个结构体开始向前遍历这个结构体链。
 输入参数  : pos     
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_entry_reverse(pos, head, member)              \
    for (pos = list_entry((head)->prev, typeof(*pos), member);      \
         list_prefetch(pos->member.prev), &pos->member != (head);   \
         pos = list_entry(pos->member.prev, typeof(*pos), member))


/*****************************************************************************
 函 数 名  : list_prepare_entry
 功能描述  : 判断pos这个指针是否为空，为空的话给它赋值list_entry(head, type-
             of(*pos), member)这条语句求出来的结构体的地址！
 输入参数  : pos     
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_prepare_entry(pos, head, member) \
    ((pos) ? : list_entry(head, typeof(*pos), member))


/*****************************************************************************
 函 数 名  : list_for_each_entry_continue
 功能描述  : 已知指向某个结构体的指针pos，以及指向它中的member成员的head指针
             ，从它的下一个结构体开始向后遍历这个链表。
 输入参数  : pos     
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_entry_continue(pos, head, member)             \
    for (pos = list_entry(pos->member.next, typeof(*pos), member);  \
         list_prefetch(pos->member.next), &pos->member != (head);   \
         pos = list_entry(pos->member.next, typeof(*pos), member))

/*****************************************************************************
 函 数 名  : list_for_each_entry_from
 功能描述  : 从pos节点开始，向后遍历链表。
 输入参数  : pos     
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_entry_from(pos, head, member)                 \
    for (; list_prefetch(pos->member.next), &pos->member != (head); \
         pos = list_entry(pos->member.next, typeof(*pos), member))


/*****************************************************************************
 函 数 名  : list_for_each_entry_safe
 功能描述  : 先保存下一个要遍历的节点！从head下一个节点向后遍历链表。
 输入参数  : pos     
             n       
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_entry_safe(pos, n, head, member)              \
    for (pos = list_entry((head)->next, typeof(*pos), member),      \
        n = list_entry(pos->member.next, typeof(*pos), member);     \
         &pos->member != (head);                                    \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))


/*****************************************************************************
 函 数 名  : list_for_each_entry_safe_continue
 功能描述  : 先保存下一个要遍历的节点！从pos下一个节点向后遍历链表。
 输入参数  : pos     
             n       
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_entry_safe_continue(pos, n, head, member)         \
    for (pos = list_entry(pos->member.next, typeof(*pos), member),      \
        n = list_entry(pos->member.next, typeof(*pos), member);         \
         &pos->member != (head);                                        \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))


/*****************************************************************************
 函 数 名  : list_for_each_entry_safe_from
 功能描述  : 先保存下一个要遍历的节点！从pos节点向后遍历链表。
 输入参数  : pos     
             n       
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_entry_safe_from(pos, n, head, member)             \
    for (n = list_entry(pos->member.next, typeof(*pos), member);        \
         &pos->member != (head);                                        \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))


/*****************************************************************************
 函 数 名  : list_for_each_entry_safe_reverse
 功能描述  : 先保存下一个要遍历的节点！从链表尾部向前遍历链表。
 输入参数  : pos     
             n       
             head    
             member  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
#define list_for_each_entry_safe_reverse(pos, n, head, member)      \
    for (pos = list_entry((head)->prev, typeof(*pos), member),      \
        n = list_entry(pos->member.prev, typeof(*pos), member);     \
         &pos->member != (head);                                    \
         pos = n, n = list_entry(n->member.prev, typeof(*n), member))

#endif
