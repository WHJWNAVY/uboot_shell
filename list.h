#ifndef _LINK_LIST_H_
#define _LINK_LIST_H_

struct list_head
{
    struct list_head *next, *prev;
};

/*****************************************************************************
 �� �� ��  : list_prefetch
 ��������  : ����cpu��ЩԪ���п������Ͼ�Ҫ�õ�������cpuԤȡһ�£�����������
             ���ٶȡ�
 �������  : const void *x  
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static inline void list_prefetch(const void *x) {;}

/*****************************************************************************
 �� �� ��  : offsetof
 ��������  : ����֪ĳһ����Ա���������ֺͽṹ�����͵�����£�����ó�Ա�����
             �ṹ�����ʼ��ַ��ƫ����.
 �������  : TYPE    :�ṹ����������
             MEMBER  :�ṹ���г�Ա
 �������  : ��
 �� �� ֵ  : �ó�Ա����ڽṹ�����ʼ��ַ��ƫ������
*****************************************************************************/
#define list_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


/*****************************************************************************
 �� �� ��  : container_of
 ��������  : ��֪ĳһ����Ա���������֡�ָ��ͽṹ�����͵�����£�����ṹ���
             ָ�룬Ҳ���Ǽ���ṹ�����ʼ��ַ��
 �������  : ptr     :ĳһ����Ա��ָ��
             type    :�ṹ������
             member  :��Ա��������
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_container_of(ptr, type, member) ({                     \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);        \
        (type *)( (char *)__mptr - list_offsetof(type,member) );})


/*****************************************************************************
 �� �� ��  : LIST_HEAD_INIT
 ��������  : ��ʼ��һ���������Ϊname��˫��ѭ�������ͷ���
 �������  : name  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define LIST_HEAD_INIT(name) { &(name), &(name) }


/*****************************************************************************
 �� �� ��  : LIST_HEAD
 ��������  : ��ʼ��һ���������Ϊname��˫��ѭ�������ͷ���
 �������  : name  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


/*****************************************************************************
 �� �� ��  : INIT_LIST_HEAD
 ��������  : ��ʼ��һ���������Ϊname��˫��ѭ�������ͷ���
 �������  : struct list_head *list  
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}


/*****************************************************************************
 �� �� ��  : __list_add
 ��������  : ��ӽ��new��prev��next֮��
 �������  : struct list_head *new   
             struct list_head *prev  
             struct list_head *next  
 �������  : ��
 �� �� ֵ  : void

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
 �� �� ��  : list_add
 ��������  : ��ӽ��new������ͷ
 �������  : struct list_head *new   
             struct list_head *head  
 �������  : ��
 �� �� ֵ  : void

*****************************************************************************/
static inline void list_add(struct list_head *new, 
                     struct list_head *head)
{
    __list_add(new, head, head->next);
}


/*****************************************************************************
 �� �� ��  : list_add_tail
 ��������  : ��ӽ��new������β
 �������  : struct list_head *new   
             struct list_head *head  
 �������  : ��
 �� �� ֵ  : void

*****************************************************************************/
static inline void list_add_tail(struct list_head *new, 
                          struct list_head *head)
{
    __list_add(new, head->prev, head);
}


/*****************************************************************************
 �� �� ��  : __list_del
 ��������  : ɾ�����
 �������  : struct list_head *prev:��ɾ������ǰ�����
             struct list_head *next:��ɾ�����������
 �������  : ��
 �� �� ֵ  : void

*****************************************************************************/
static inline void __list_del(struct list_head *prev, 
                       struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}


/*****************************************************************************
 �� �� ��  : list_del
 ��������  : ɾ�����entry����entry��prevָ��NULL��nextָ��NULL.
 �������  : struct list_head *entry:��ɾ�����
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = /*LIST_POISON1*/NULL;
    entry->prev = /*LIST_POISON2*/NULL;
}


/*****************************************************************************
 �� �� ��  : list_replace
 ��������  : �ý��new�滻���old
 �������  : struct list_head *old  
             struct list_head *new  
 �������  : ��
 �� �� ֵ  : void
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
 �� �� ��  : list_replace_init
 ��������  : �ý��new�滻���old������ʼ��old.
 �������  : struct list_head *old  
             struct list_head *new  
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static inline void list_replace_init(struct list_head *old,
                              struct list_head *new)
{
    list_replace(old, new);
    INIT_LIST_HEAD(old);
}


/*****************************************************************************
 �� �� ��  : list_del_init
 ��������  : ɾ�����entry������ʼ��entry.
 �������  : struct list_head *entry  
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}


/*****************************************************************************
 �� �� ��  : list_move
 ��������  : �Ƚ�list�ڵ��ԭ������ɾ����Ȼ������ӵ�head����ı�ͷ.
 �������  : struct list_head *list  
             struct list_head *head  
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static inline void list_move(struct list_head *list, 
                      struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}


/*****************************************************************************
 �� �� ��  : list_move_tail
 ��������  : �Ƚ�list�ڵ��ԭ������ɾ����Ȼ������ӵ�head����ı�β.
 �������  : struct list_head *list  
             struct list_head *head  
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static inline void list_move_tail(struct list_head *list,
                           struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}


/*****************************************************************************
 �� �� ��  : list_is_last
 ��������  : ����list�ڵ��Ƿ�Ϊhead����ı�β�ڵ㡣�Ƿ���1�����򷵻�0.
 �������  : const struct list_head *list  
             const struct list_head *head  
 �������  : ��
 �� �� ֵ  : 1/0
*****************************************************************************/
static inline int list_is_last(const struct list_head *list,
                        const struct list_head *head)
{
    return list->next == head;
}


/*****************************************************************************
 �� �� ��  : list_empty
 ��������  : �ж�head�����Ƿ�Ϊ�������Ƿ���1�����򷵻�Ϊ0.
 �������  : const struct list_head *head  
 �������  : ��
 �� �� ֵ  : 1/0
*****************************************************************************/
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}


/*****************************************************************************
 �� �� ��  : list_empty_careful
 ��������  : �жϽڵ�head��ǰ���ͺ����Ƿ�ָ��head���Ƿ���1�����򷵻�0.
 �������  : const struct list_head *head  
 �������  : ��
 �� �� ֵ  : 1/0
*****************************************************************************/
static inline int list_empty_careful(const struct list_head *head)
{
    struct list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

/*****************************************************************************
 �� �� ��  : __list_splice
 ��������  : ��list�����ȫ���ڵ㣨ͷ�ڵ�list���⣩������prev��next�ڵ�֮��.
             
 �������  : struct list_head *list  
             struct list_head *head  
 �������  : ��
 �� �� ֵ  : void
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
 �� �� ��  : list_splice
 ��������  : ��list�Ƿǿ����������£��������head�����ͷ������head�ڵ��
             ���档�˺�������ȫ����Ϊ�ڲ������ͨ��list�ڵ���ʵ�����Ľ�
             ��.
 �������  : struct list_head *list  
             struct list_head *head  
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static inline void list_splice(struct list_head *list, 
                        struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head);
}


/*****************************************************************************
 �� �� ��  : list_splice_init
 ��������  : ��list�Ƿǿ����������£��������head�����β������head�ڵ��
             ǰ�档Ȼ���list�ڵ���г�ʼ�����ų�����ȫ����.
 �������  : struct list_head *list  
             struct list_head *head  
 �������  : ��
 �� �� ֵ  : void
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
 �� �� ��  : list_entry
 ��������  : ��ȡtype���ͽṹ�����ʼָ��
 �������  : ptr     :type���͵Ľṹ����member��Ա��ָ��
             type    :�ṹ������
             member  :�ṹ���г�Ա
 �������  : ��
 �� �� ֵ  : �ṹ�����ʼָ��
*****************************************************************************/
#define list_entry(ptr, type, member) \
    list_container_of(ptr, type, member)


/*****************************************************************************
 �� �� ��  : list_for_each
 ��������  : ��head�ڵ㿪ʼ��������head�ڵ㣡����������ÿһ���ڵ�.
 �������  : pos   :ѭ��ָ��
             head  :����ͷ
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each(pos, head)                                        \
    for (pos = (head)->next; list_prefetch(pos->next), pos != (head);   \
            pos = pos->next)

/*****************************************************************************
 �� �� ��  : __list_for_each
 ��������  : ��head�ڵ㿪ʼ��������head�ڵ㣩��������ÿһ���ڵ�.
 �������  : pos   
             head  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define __list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)


/**
 * list_for_each_prev   -   iterate over a list backwards
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */

/*****************************************************************************
 �� �� ��  : list_for_each_prev
 ��������  : ��Ҳ�Ǵ�head�ڵ㿪ʼ��������head�ڵ㣩��ǰ����ÿһ���ڵ�.����
             �����β����ʼ����.
 �������  : pos   
             head  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_prev(pos, head)                                   \
    for (pos = (head)->prev; list_prefetch(pos->prev), pos != (head);   \
            pos = pos->prev)


/*****************************************************************************
 �� �� ��  : list_for_each_safe
 ��������  : ��head�ڵ㿪ʼ��������head�ڵ㣩��������ÿһ���ڵ�.����n�Ƚ�
             ��һ��Ҫ�����Ľڵ㱣����������ֹɾ�����ڵ���޷��ҵ���һ����
             �㣬�����ִ���.
 �������  : pos   
             n     
             head  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_safe(pos, n, head)                        \
    for (pos = (head)->next, n = pos->next; pos != (head);      \
        pos = n, n = pos->next)


/*****************************************************************************
 �� �� ��  : list_for_each_entry
 ��������  : ��ָ֪��ĳ���ṹ���ָ��pos���Լ�ָ������member��Ա��ָ��head��
             ����һ���ṹ�忪ʼ����������ṹ������
 �������  : pos     
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_entry(pos, head, member)                      \
    for (pos = list_entry((head)->next, typeof(*pos), member);      \
         list_prefetch(pos->member.next), &pos->member != (head);   \
         pos = list_entry(pos->member.next, typeof(*pos), member))


/*****************************************************************************
 �� �� ��  : list_for_each_entry_reverse
 ��������  : ��ָ֪��ĳ���ṹ���ָ��pos���Լ�ָ������member��Ա��ָ��head��
             ����һ���ṹ�忪ʼ��ǰ��������ṹ������
 �������  : pos     
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_entry_reverse(pos, head, member)              \
    for (pos = list_entry((head)->prev, typeof(*pos), member);      \
         list_prefetch(pos->member.prev), &pos->member != (head);   \
         pos = list_entry(pos->member.prev, typeof(*pos), member))


/*****************************************************************************
 �� �� ��  : list_prepare_entry
 ��������  : �ж�pos���ָ���Ƿ�Ϊ�գ�Ϊ�յĻ�������ֵlist_entry(head, type-
             of(*pos), member)�������������Ľṹ��ĵ�ַ��
 �������  : pos     
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_prepare_entry(pos, head, member) \
    ((pos) ? : list_entry(head, typeof(*pos), member))


/*****************************************************************************
 �� �� ��  : list_for_each_entry_continue
 ��������  : ��ָ֪��ĳ���ṹ���ָ��pos���Լ�ָ�����е�member��Ա��headָ��
             ����������һ���ṹ�忪ʼ�������������
 �������  : pos     
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_entry_continue(pos, head, member)             \
    for (pos = list_entry(pos->member.next, typeof(*pos), member);  \
         list_prefetch(pos->member.next), &pos->member != (head);   \
         pos = list_entry(pos->member.next, typeof(*pos), member))

/*****************************************************************************
 �� �� ��  : list_for_each_entry_from
 ��������  : ��pos�ڵ㿪ʼ������������
 �������  : pos     
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_entry_from(pos, head, member)                 \
    for (; list_prefetch(pos->member.next), &pos->member != (head); \
         pos = list_entry(pos->member.next, typeof(*pos), member))


/*****************************************************************************
 �� �� ��  : list_for_each_entry_safe
 ��������  : �ȱ�����һ��Ҫ�����Ľڵ㣡��head��һ���ڵ�����������
 �������  : pos     
             n       
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_entry_safe(pos, n, head, member)              \
    for (pos = list_entry((head)->next, typeof(*pos), member),      \
        n = list_entry(pos->member.next, typeof(*pos), member);     \
         &pos->member != (head);                                    \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))


/*****************************************************************************
 �� �� ��  : list_for_each_entry_safe_continue
 ��������  : �ȱ�����һ��Ҫ�����Ľڵ㣡��pos��һ���ڵ�����������
 �������  : pos     
             n       
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_entry_safe_continue(pos, n, head, member)         \
    for (pos = list_entry(pos->member.next, typeof(*pos), member),      \
        n = list_entry(pos->member.next, typeof(*pos), member);         \
         &pos->member != (head);                                        \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))


/*****************************************************************************
 �� �� ��  : list_for_each_entry_safe_from
 ��������  : �ȱ�����һ��Ҫ�����Ľڵ㣡��pos�ڵ�����������
 �������  : pos     
             n       
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_entry_safe_from(pos, n, head, member)             \
    for (n = list_entry(pos->member.next, typeof(*pos), member);        \
         &pos->member != (head);                                        \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))


/*****************************************************************************
 �� �� ��  : list_for_each_entry_safe_reverse
 ��������  : �ȱ�����һ��Ҫ�����Ľڵ㣡������β����ǰ��������
 �������  : pos     
             n       
             head    
             member  
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
#define list_for_each_entry_safe_reverse(pos, n, head, member)      \
    for (pos = list_entry((head)->prev, typeof(*pos), member),      \
        n = list_entry(pos->member.prev, typeof(*pos), member);     \
         &pos->member != (head);                                    \
         pos = n, n = list_entry(n->member.prev, typeof(*n), member))

#endif
