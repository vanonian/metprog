template <typename... Tail>
class ArgsPack;

template <>
class ArgsPack<> {
public:
  ArgsPack() {}

  template <typename Delegate, typename... Args>
  auto Call(const Delegate& delegate, Args&&... args)
    -> decltype(delegate(args...)) {
    return delegate(args...);
  }
};

template<typename Head, typename... Tail>
class ArgsPack<Head, Tail...> : ArgsPack<Tail...> {
private:
  Head m_Param;
  typedef ArgsPack<Tail...> __base;

  template<typename T>
  T* ConvertToPtr(T& t) {
      return &t;
  }

  template<typename T>
  T* ConvertToPtr(T* t) {
      return t;
  }

  template<typename T>
  T& ConvertToRef(T& t) {
      return t;
  }


  template<typename T>
  T& ConvertToRef(T* t) {
      return *t;
  }

public:
  ArgsPack(Head head, Tail... tail) : __base(tail...), m_Param(head) {}

  template <typename Delegate, typename... Args>
  auto Call(const Delegate& delegate, Args&&... args)
    -> decltype(this->__base::Call(delegate, args..., ConvertToRef(m_Param))) {
    return __base::Call(delegate, args..., ConvertToRef(m_Param));
  }

  template <typename Delegate, typename... Args>
  auto Call(const Delegate& delegate, Args&&... args)
    -> decltype(this->__base::Call(delegate, args..., ConvertToPtr(m_Param))) {
    return __base::Call(delegate, args..., ConvertToPtr(m_Param));
  }
};


template <typename TObject, typename TCE>
struct SimpleDelegate {
private:
  TObject& m_Object;
  TCE m_CallableEntity;

public:
  SimpleDelegate(TObject& object, const TCE& ptr)
    : m_Object(object), m_CallableEntity(ptr) {}

  template <typename... Args>
  auto operator()(Args... args) const
    -> decltype((m_Object.*m_CallableEntity)(args...)) {
    return (m_Object.*m_CallableEntity)(args...);
  }
};

template <typename TCE>
struct CEWrapper {
private:
  TCE m_CallableEntity;

public:
  CEWrapper(const TCE& ptr)
    : m_CallableEntity(ptr) {}

  template <typename... Args>
  auto operator()(Args... args) const -> decltype(m_CallableEntity(args...)) {
    return m_CallableEntity(args...);
  }
};

template <typename TCallableEntity, typename... Args>
class Functor : ArgsPack<Args...> {
private:
  typedef ArgsPack<Args...> __base;
  TCallableEntity m_CallableEntity;

public:
  Functor(TCallableEntity callableEntity, Args... args)
    : __base(args...), m_CallableEntity(callableEntity) {}

  template <typename TCE = TCallableEntity>
  auto  Call() -> decltype(this->__base::Call(CEWrapper<TCE>(m_CallableEntity))) const {
    return __base::Call(m_CallableEntity);
  }

  template <typename Object>
  auto Call(Object& object)
    -> decltype(this->__base::Call(SimpleDelegate<Object, TCallableEntity>(object,
                                                                             m_CallableEntity))) const {
    return __base::Call(SimpleDelegate<Object, TCallableEntity>(object, m_CallableEntity));
  }
};


template <typename TCallableEntity, typename... Args>
Functor<TCallableEntity, Args...> make_functor(const TCallableEntity& callable_entity,
                                               Args&&... args) {
  return Functor<TCallableEntity, Args...>(callable_entity, std::forward<Args>(args)...);
}
