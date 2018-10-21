// Copyright (c) 2018, Peter Ohler, All rights reserved.

#include <stdlib.h>
#include <string.h>

#include "gqlintro.h"
#include "gqlvalue.h"
#include "graphql.h"

static gqlType	schema_type;
static gqlType	type_type;
static gqlType	type_kind_type;
static gqlType	field_type;
static gqlType	input_value_type;
static gqlType	enum_value_type;
static gqlType	directive_type;
static gqlType	directive_location_type;

// type __Schema {
//   types: [__Type!]!
//   queryType: __Type!
//   mutationType: __Type
//   subscriptionType: __Type
//   directives: [__Directive!]!
// }
static gqlRef
schema_types_resolve(gqlRef target, const char *fieldName) {
    // TBD 
    return NULL;
}

static gqlRef
schema_query_type_resolve(gqlRef target, const char *fieldName) {
    // TBD return schema_type.query.type
    //  lookup "schema" type
    //  get query field
    //  get return type of field
    return NULL;
}

static int
create_schema_type(Err err) {
    if (NULL == (schema_type = gql_type_create(err, "__Schema", NULL, true, NULL)) ||
	NULL == gql_type_field(err, schema_type, "types", type_type, NULL, true, true, true, schema_types_resolve) ||
	NULL == gql_type_field(err, schema_type, "queryType", type_type, NULL, true, false, false, schema_query_type_resolve) ||
	NULL == gql_type_field(err, schema_type, "mutationType", type_type, NULL, false, false, false, NULL) ||
	NULL == gql_type_field(err, schema_type, "subscriptionType", type_type, NULL, false, false, false, NULL) ||
	NULL == gql_type_field(err, schema_type, "directives", directive_type, NULL, true, true, true, NULL)) {

	return err->code;
    }
    return ERR_OK;
}

// type __Type {
//   kind: __TypeKind!
//   name: String
//   description: String
//   fields(includeDeprecated: Boolean = false): [__Field!]
//   interfaces: [__Type!]
//   possibleTypes: [__Type!]
//   enumValues(includeDeprecated: Boolean = false): [__EnumValue!]
//   inputFields: [__InputValue!]
//   ofType: __Type
// }
static int
create_type_type(Err err) {
    gqlField	fields = NULL;
    gqlField	enum_values = NULL;
    gqlValue	dv;
    
    if (NULL == (type_type = gql_type_create(err, "__Type", NULL, true, NULL)) ||
	NULL == gql_type_field(err, type_type, "kind", type_kind_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, type_type, "name", &gql_string_type, NULL, false, false, false, NULL) ||
	NULL == gql_type_field(err, type_type, "description", &gql_string_type, NULL, false, false, false, NULL) ||
	NULL == (fields = gql_type_field(err, type_type, "fields", field_type, NULL, false, true, true, NULL)) ||
	NULL == gql_type_field(err, type_type, "interfaces", type_type, NULL, false, true, true, NULL) ||
	NULL == gql_type_field(err, type_type, "possibleTypes", type_type, NULL, false, true, true, NULL) ||
	NULL == (enum_values = gql_type_field(err, type_type, "enumValues", enum_value_type, NULL, false, true, true, NULL)) ||
	NULL == gql_type_field(err, type_type, "inputFields", input_value_type, NULL, false, true, true, NULL) ||
	NULL == gql_type_field(err, type_type, "ofType", type_type, NULL, false, false, false, NULL)) {

	return err->code;
    }
    if (NULL == (dv = gql_bool_create(err, false)) ||
	NULL == gql_field_arg(err, fields, "includeDeprecated", &gql_bool_type, "A comment.", dv, false) ||
	NULL == (dv = gql_bool_create(err, false)) ||
	NULL == gql_field_arg(err, enum_values, "includeDeprecated", &gql_bool_type, "    One line\n    and two.", dv, false)) {

	return err->code;
    }
    return ERR_OK;
}

static int
create_type_kind_type(Err err) {
    const char	*choices[] = {
	"SCALAR",
	"OBJECT",
	"INTERFACE",
	"UNION",
	"ENUM",
	"INPUT_OBJECT",
	"LIST",
	"NON_NULL",
	NULL
    };

    type_kind_type = gql_enum_create(err, "__TypeKind", NULL, true, choices);
    
    return err->code;
}

// type __Field {
//   name: String!
//   description: String
//   args: [__InputValue!]!
//   type: __Type!
//   isDeprecated: Boolean!
//   deprecationReason: String
// }
static int
create_field_type(Err err) {
    if (NULL == (field_type = gql_type_create(err, "__Field", NULL, true, NULL)) ||
	NULL == gql_type_field(err, field_type, "name", &gql_string_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, field_type, "description", &gql_string_type, NULL, false, false, false, NULL) ||
	NULL == gql_type_field(err, field_type, "args", input_value_type, NULL, true, true, true, NULL) ||
	NULL == gql_type_field(err, field_type, "type", type_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, field_type, "isDeprecated", &gql_bool_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, field_type, "reason", &gql_string_type, NULL, false, false, false, NULL)) {

	return err->code;
    }
    return ERR_OK;
}

// type __InputValue {
//   name: String!
//   description: String
//   type: __Type!
//   defaultValue: String
// }
static int
create_input_type(Err err) {
    if (NULL == (input_value_type = gql_type_create(err, "__InputValue", NULL, true, NULL)) ||
	NULL == gql_type_field(err, input_value_type, "name", &gql_string_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, input_value_type, "description", &gql_string_type, NULL, false, false, false, NULL) ||
	NULL == gql_type_field(err, input_value_type, "type", type_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, input_value_type, "defaultValue", &gql_string_type, NULL, false, false, false, NULL)) {

	return err->code;
    }
    return ERR_OK;
}

// type __EnumValue {
//   name: String!
//   description: String
//   isDeprecated: Boolean!
//   deprecationReason: String
// }
static int
create_enum_type(Err err) {
    if (NULL == (enum_value_type = gql_type_create(err, "__EnumValue", NULL, true, NULL)) ||
	NULL == gql_type_field(err, enum_value_type, "name", &gql_string_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, enum_value_type, "description", &gql_string_type, NULL, false, false, false, NULL) ||
	NULL == gql_type_field(err, enum_value_type, "isDeprecated", &gql_bool_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, enum_value_type, "deprecationReason", &gql_string_type, NULL, false, false, false, NULL)) {

	return err->code;
    }
    return ERR_OK;
}

// type __Directive {
//   name: String!
//   description: String
//   locations: [__DirectiveLocation!]!
//   args: [__InputValue!]!
// }
static int
create_directive_type(Err err) {
    if (NULL == (directive_type = gql_type_create(err, "__Directive", NULL, true, NULL)) ||
	NULL == gql_type_field(err, directive_type, "name", &gql_string_type, NULL, true, false, false, NULL) ||
	NULL == gql_type_field(err, directive_type, "description", &gql_string_type, NULL, false, false, false, NULL) ||
	NULL == gql_type_field(err, directive_type, "location", directive_location_type, NULL, true, true, true, NULL) ||
	NULL == gql_type_field(err, directive_type, "args", input_value_type, NULL, true, true, true, NULL)) {

	return err->code;
    }
    return ERR_OK;
}

static int
create_directive_location_type(Err err) {
    const char	*choices[] = {
	"QUERY",
	"MUTATION",
	"SUBSCRIPTION",
	"FIELD",
	"FRAGMENT_DEFINITION",
	"FRAGMENT_SPREAD",
	"INLINE_FRAGMENT",
	"SCHEMA",
	"SCALAR",
	"OBJECT",
	"FIELD_DEFINITION",
	"ARGUMENT_DEFINITION",
	"INTERFACE",
	"UNION",
	"ENUM",
	"ENUM_VALUE",
	"INPUT_OBJECT",
	"INPUT_FIELD_DEFINITION",
	NULL };

    directive_location_type = gql_enum_create(err, "__DirectiveLocation", NULL, true, choices);
    
    return err->code;
}
    
int
gql_intro_init(Err err) {
    gqlField	f;
    
    if (ERR_OK != create_type_kind_type(err) ||
	ERR_OK != create_input_type(err) ||
	ERR_OK != create_type_type(err) ||
	ERR_OK != create_enum_type(err) ||
	ERR_OK != create_field_type(err) ||
	ERR_OK != create_directive_location_type(err) ||
	ERR_OK != create_directive_type(err) ||
	ERR_OK != create_schema_type(err)) {
	return err->code;
    }
    // InputValue and Type as well Field and Type as have a depency loop so
    // complete the loop.
    for (f = input_value_type->fields; NULL != f; f = f->next) {
	if (0 == strcmp("type", f->name)) {
	    f->type = type_type;
	    break;
	}
    }
    for (f = type_type->fields; NULL != f; f = f->next) {
	if (0 == strcmp("fields", f->name)) {
	    f->type = field_type;
	} else if (0 == strcmp("enumValues", f->name)) {
	    f->type = enum_value_type;
	    break;
	}
    }
    return ERR_OK;
}
