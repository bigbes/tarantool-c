#ifndef TNT_SCHEMA_H_INCLUDED
#define TNT_SCHEMA_H_INCLUDED

/*
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the
 *    following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \internal
 * \file tnt_schema.h
 * \brief Tarantool schema
 */

/**
 * \internal
 * \brief types in schema/indexes
 */
enum field_type {
	FT_STR = 0,
	FT_NUM,
	FT_OTHER
};

/**
 * \internal
 * \brief field value information
 */
struct tnt_schema_fval {
	uint32_t        field_number;
	char           *field_name;
	uint32_t        field_name_len;
	enum field_type field_type;
};

/**
 * \internal
 * \brief index value information
 */
struct tnt_schema_ival {
	char      *index_name;
	uint32_t   index_name_len;
	uint32_t   index_number;
	uint32_t   index_parts_len;
	struct tnt_schema_fval *index_parts;
};

/**
 * \internal
 * \brief space value information
 */
struct tnt_schema_sval {
	char      *space_name;
	uint32_t   space_name_len;
	uint32_t   space_number;
	uint32_t   schema_list_len;
	struct tnt_schema_fval *schema_list;
	struct mh_assoc_t      *index_hash;
};

/**
 * \brief Schema of tarantool instance
 */
struct tnt_schema {
	struct mh_assoc_t *space_hash; /*!< hash with spaces */
	int alloc; /*!< allocation mark */
};

/**
 * \brief Add spaces definitions to schema
 *
 * must be used with response's data of select from 280/281 spaces
 * (_space/_vspace)
 *
 * \param sch  schema pointer
 * \param data pointer to data
 * \param dlen data size
 *
 * \returns status
 * \retval  -1 failed parsing/oom
 * \retval  0 ok
 */
int tnt_schema_add_spaces  (struct tnt_schema *sch, const char *data, uint32_t dlen);
/**
 * \brief Add indexes definitions to schema
 *
 * must be used with response's data of select from 288/289 indexes
 * (_index/_vindex)
 *
 * \param sch  schema pointer
 * \param data pointer to data
 * \param dlen data size
 *
 * \returns status
 * \retval  -1 failed parsing/oom
 * \retval  0 ok
 */
int tnt_schema_add_indexes (struct tnt_schema *sch, const char *data, uint32_t dlen);

/**
 * \brief Get spaceno by space name
 *
 * \param sch   schema pointer
 * \param sstr  space name
 * \param sslen space name len
 *
 * \returns space id
 * \retval -1 error, space not found
 */
int32_t
tnt_schema_stosid (struct tnt_schema *sch, const char *sstr, uint32_t sslen);
/**
 * \brief Get indexno by space no and index name
 *
 * \param sch   schema pointer
 * \param sno   space no
 * \param istr  index name
 * \param islen index name len
 *
 * \returns index id
 * \retval -1 error, index/space not found
 */
int32_t
tnt_schema_stoiid (struct tnt_schema *sch, uint32_t sno, const char *istr,
		   uint32_t islen);

/**
 * \brief Create and init schema object
 *
 * if schema pointer is NULL, then new schema object will be created
 *
 * \param sch schema pointer
 * \returns new schema object
 * \retval  NULL oom
 */
struct tnt_schema *tnt_schema_new(struct tnt_schema *sch);
/**
 * \brief Reset schema to default state (empty)
 * \param sch schema pointer
 */
void tnt_schema_flush  (struct tnt_schema *sch);
/**
 * \brief Reset and free schema
 * \param sch schema pointer
 */
void tnt_schema_delete (struct tnt_schema *sch);

#endif /* TNT_SCHEMA_H_INCLUDED */
