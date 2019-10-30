struct NIL {
};

template <class Head, class Tail=NIL>
struct TypeList {
    typedef Head head;
    typedef Tail tail;
};

template <int Pos, class T, class TList>
struct IndexOf {
	static const int index = IndexOf<Pos + 1, T, class TList::tail>::index;
};

template <int Pos, class T, class Tail>
struct IndexOf <Pos, T, TypeList<T, Tail> > {
	static const int index = Pos;
};

template <int Pos, class T>
struct IndexOf <Pos, T, TypeList <T, NIL> > {
	static const int index = Pos;
};

template <int Pos, class T, class Head>
struct IndexOf <Pos, T, TypeList<Head, NIL> > {
	static const int index = -1;
};


template <class T, class TList>
struct getIndexOf {
    static const int result = IndexOf<0, T, TList>::index;
};
