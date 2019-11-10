struct NIL {
};


template <class H, class T=NIL>
struct TypeList {
    typedef H Head;
    typedef T Tail;
};


template<class TList, template<class> class Unit>
class GenScatterHierarchy;

template<class T1, class T2, template<class> class Unit>
class GenScatterHierarchy<TypeList <T1, T2>, Unit>
      : public GenScatterHierarchy<T1, Unit>,
        public GenScatterHierarchy<T2, Unit> {
 public:
  using TList = TypeList <T1, T2>;
  using LeftBase = GenScatterHierarchy<T1, Unit>;
  using RightBase = GenScatterHierarchy<T2, Unit>;
};

template<class AtomicType, template<class> class Unit>
class GenScatterHierarchy : public Unit<AtomicType> {
 public:
  using TList = TypeList <AtomicType, NIL>;
  using LeftBase = Unit<AtomicType>;
};

template<template<class> class Unit>
class GenScatterHierarchy<NIL, Unit> {
};


template <class T, class Head, class Tail, class H>
struct UnitOfType {
  typedef typename UnitOfType<T,
                              typename H::RightBase::TList::Head,
                              typename H::RightBase::TList::Tail,
                              typename H::RightBase>::UnitT UnitT;
};

template <class T, class Tail, class H>
struct UnitOfType<T, T, Tail, H> {
  typedef typename H::LeftBase UnitT;
};


template <int Pos, class H>
struct UnitOfTypeAt  {
  typedef typename UnitOfTypeAt <Pos - 1, typename H::RightBase>::UnitT UnitT;
};

template <class H>
struct UnitOfTypeAt<0, H> {
  typedef typename H::LeftBase UnitT;
};


template<class T, class H>
typename UnitOfType<T,
                    typename H::TList::Head,
                    typename H::TList::Tail,
                    H>::UnitT& Field(H& hierarchy) {
  return hierarchy;
}

template<int Pos, class H>
typename UnitOfTypeAt<Pos, H>::UnitT& Field(H& hierarchy) {
  return hierarchy;
}
