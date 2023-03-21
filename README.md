# Single linked list template class.
## Упрощенная реализация std::forward_list стандартной шаблонной библиотеки.

Односвязный список, поддерживающий быструю вставку и удаление элементов из любого места списка, при этом не инвалидирующий уже инициализированные итераторы. Проект разрабатывался с целью изучения устройства одного из контейнеров стандатной шаблонной библиотеки а также возможностей языка С++.

Односвязный список допускает следующие операции:
- [*Конструирование списка*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L60) с указанием содержимого (*std::initializer_list*);
- [*Копирование*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L65) и [*присваивание*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L75);
- [*PushFront*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L92) - вставка элемента в начало списка;
- [*InsertAfter*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L107) - вставка элемента после некоторого элемента списка;
- [*PopFront*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L98) - удаление элемента из начала списка;
- [*EraseAfter*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L115) - удаление элемента, следующего за данным элементом списка;
- [*IsEmpty*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L39) - проверка списка на пустоту;
- [*GetSize*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L38) - определение количества элементов в списке;
- [*Clear*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L82) - очистка двусвязного списка;
- [*swap*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L125) - обмен содержимого списков;
- [*Методы, возвращающие итераторы*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L41):
    - Операции вставки и удаления элемента из односвязного списка требуют указателя на элемент, предшествующий позиции вставки/удаления. В этих операциях такой объект используется как опорный. Поэтому помимо *begin* и *end*, присущих другим контейнерам, односвязному списка нужен итератор *before_begin*. Данный итератор ссылается на позицию перед первым элементом.
- [*Сравнение списков операторами*](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L147): '<', '>', '==', '<=', '>=', '!='.

Шаблон класса [**BasicIterator**](https://github.com/konstantinbelousovEC/cpp-single-linked-list/blob/f1ac0436b2e838ce17ee20c2e3b8207ee593f1ce/single-linked-list/single-linked-list.h#L188) определяет поведение итератора на элементы односвязного списка *ValueType* — совпадает с *Type* (для *Iterator*) либо с *const Type* (для *ConstIterator*)

По умолчанию класс *SingleLinkedList* не имеет доступа к приватной области своего итератора. Объявив список [*friend*](https://disk.yandex.ru/i/JmF4shw20tUnGQ), итератор разрешит ему, и только ему обращаться к своей внутренней части. В данном случае такое доверие обоснованно, так как и список, и итератор работают с одной и той же структурой данных, скрытой от внешнего мира.

Достоинства односвязного списка:
- вставка и удаление элемента выполняются за константное время, то есть не зависят от количества элементов и позиции вставляемого или удаляемого элемента;
- размер списка ограничен лишь объёмом доступной памяти.

Недостатки односвязного списка следуют из особенностей его структуры:
- узнать адрес элемента по его порядковому номеру — операция линейной сложности. Чтобы определить адрес N-го элемента списка, нужно последовательно перебрать все N-1 элементов, начиная с первого элемента.
- неэффективное расходование памяти: помимо данных, каждый элемент списка хранит указатель на следующий элемент. Кроме того, при каждом создании объекта в динамической памяти пара десятков байт расходуется на поддержание структуры кучи.
- не такая высокая эффективность вставки и удаления. Каждая вставка и каждое удаление обращаются к операциям работы с кучей: new или delete. Считается, что эти операции работают за константное время, однако константа может быть достаточно большой. При этом выполняется сложный код синхронизации между потоками, и могут быть задействованы низкоуровневые механизмы работы с памятью.
- соседние элементы списка могут располагаться в памяти непоследовательно, что снижает эффективность работы кэш-памяти.

## Сборка, установка и системные требования.

Тривиальное использование - скопировать и вставить в свой проект заголовочный файл single-linked-list.h. Для успешной сборки необходим любой современный компилятор с поддержкой стандрта C++14 и выше.

## Планы по расширению функционала.

- Merge - слияние двух отсортированных списокв;
- Remove - удаление элемента, соответсвующего значению;
- RemoveIf - удаление элемента, соответсвующего условию;
- Reverse - обращение односвязного списка;
- Unique - удалению дублирующихся элементов;
- Sort - сортировка списка.