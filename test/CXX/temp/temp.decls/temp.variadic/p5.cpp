// RUN: %clang_cc1 -std=c++0x -fblocks -fsyntax-only -verify %s


// An appearance of a name of a parameter pack that is not expanded is
// ill-formed.

template<typename T, typename U> struct pair;

// Test for unexpanded parameter packs in each of the type nodes.
template<typename T, int N, typename ... Types>
struct TestPPName 
  : public Types, public T  // expected-error{{base type contains unexpanded parameter pack 'Types'}}
{
  // BuiltinType is uninteresting
  // FIXME: ComplexType is uninteresting?
  // PointerType
  typedef Types *types_pointer; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}

  // BlockPointerType
  typedef Types (^block_pointer_1)(int); // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}
  typedef int (^block_pointer_2)(Types); // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}

  // LValueReferenceType
  typedef Types &lvalue_ref; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}

  // RValueReferenceType
  typedef Types &&rvalue_ref; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}

  // MemberPointerType
  typedef Types TestPPName::* member_pointer_1; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}
  typedef int Types::*member_pointer_2; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // ConstantArrayType
  typedef Types constant_array[17]; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // IncompleteArrayType
  typedef Types incomplete_array[]; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // VariableArrayType
  void f(int i) {
    Types variable_array[i]; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 
  }

  // DependentSizedArrayType
  typedef Types dependent_sized_array[N]; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // DependentSizedExtVectorType
  typedef Types dependent_sized_ext_vector __attribute__((ext_vector_type(N))); // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // VectorType is uninteresting

  // ExtVectorType
  typedef Types ext_vector __attribute__((ext_vector_type(4))); // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // FunctionProtoType
  typedef Types (function_type_1)(int); // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 
  typedef int (function_type_2)(Types); // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // FunctionNoProtoType is uninteresting
  // UnresolvedUsingType is uninteresting
  // ParenType is uninteresting
  // TypedefType is uninteresting

  // TypeOfExprType
  typedef __typeof__((static_cast<Types>(0))) typeof_expr; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // TypeOfType
  typedef __typeof__(Types) typeof_type;  // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // DecltypeType
  typedef decltype((static_cast<Types>(0))) typeof_expr; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // RecordType is uninteresting
  // EnumType is uninteresting
  // ElaboratedType is uninteresting

  // TemplateTypeParmType
  typedef Types template_type_parm; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // SubstTemplateTypeParmType is uninteresting

  // TemplateSpecializationType
  typedef pair<Types, int> template_specialization; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // InjectedClassName is uninteresting.

  // DependentNameType
  typedef typename Types::type dependent_name; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // DependentTemplateSpecializationType
  typedef typename Types::template apply<int> dependent_name_1; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 
  typedef typename T::template apply<Types> dependent_name_2; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}} 

  // ObjCObjectType is uninteresting
  // ObjCInterfaceType is uninteresting
  // ObjCObjectPointerType is uninteresting
};

// FIXME: Test for unexpanded parameter packs in each of the expression nodes.

template<typename ... Types>
void TestPPNameFunc(int i) {
  f(static_cast<Types>(i)); // expected-error{{expression contains unexpanded parameter pack 'Types'}}
}

// FIXME: Test for unexpanded parameter packs in declarations.
template<typename... Types>
struct TestUnexpandedDecls {
  void member_function(Types);  // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}
  void member_function () throw(Types); // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}
  Types data_member;  // expected-error{{data member type contains unexpanded parameter pack 'Types'}}
  static Types static_data_member; // expected-error{{declaration type contains unexpanded parameter pack 'Types'}}
  unsigned bit_field : static_cast<Types>(0);  // expected-error{{bit-field size contains unexpanded parameter pack 'Types'}}
};

// Test for diagnostics in the presence of multiple unexpanded
// parameter packs.
template<typename T, typename U> struct pair;

template<typename ...OuterTypes>
struct MemberTemplatePPNames {
  template<typename ...InnerTypes>
  struct Inner {
    typedef pair<OuterTypes, InnerTypes>* types; // expected-error{{declaration type contains unexpanded parameter packs 'OuterTypes' and 'InnerTypes'}}

    template<typename ...VeryInnerTypes>
    struct VeryInner {
      typedef pair<pair<VeryInnerTypes, OuterTypes>, pair<InnerTypes, OuterTypes> > types; // expected-error{{declaration type contains unexpanded parameter packs 'VeryInnerTypes', 'OuterTypes', ...}}
    };
  };
};
