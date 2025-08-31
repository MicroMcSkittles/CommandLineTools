#pragma once
#include <vector>
#include <string>
#include <stdint.h>

namespace CLT {

	//enum class TableFormatFlags {
	//	None = 0,
	//	FixedFieldWidth = 1 << 0,
	//};
	struct TableFormat {
		char edgeVert     = '|'; // The character that will be used for vertical edges of the table
		char edgeHori     = '-'; // The character that will be used for horisontal edges of the table
		char intersection = '+'; // The character that will be used any where a vertical and horisontal edge intersect
	};

	enum class FieldAlignment {
		None = 0,
		Left,
		Middle,
		Right
	};
	struct TableField {
		const char*    name      = "";                   // The displayed name of the field
		uint16_t       width     = 0;                    // The min width of the field, found automaticaly if 0
		FieldAlignment alignment = FieldAlignment::Left; // Tells the table to display field entries to the left, middle, or right
	
		TableField(const char* name, uint16_t width = 0, FieldAlignment alignment = FieldAlignment::Left) :
			name(name), width(width), alignment(alignment) { }
	};

	struct Table {
		const char*              name = "";
		std::vector<TableField>  fields;
		std::vector<const char*> entries;
		TableFormat              format;

		void operator << (const char* newEntry);
		void operator << (std::vector<const char*> newEntries);
	};

	Table       CreatTable(const char* name, const std::vector<TableField>& fields, const TableFormat& format = { });
	std::string GenerateTableString(const Table& table);
	void        PrintTable(const Table& table);
}