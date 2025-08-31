#include "CommandLineTools/IOFormat.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>

namespace CLT {

	void Table::operator<<(const char* newEntry)
	{
		// Get the field we are pushing to
		size_t fieldIndex = entries.size() % fields.size();
		TableField& field = fields[fieldIndex];

		// Get the new field width, make sure it isn't wider then the largest unsigned 16 bit int
		uint16_t entrySize = std::min((size_t)((uint16_t)-1), strlen(newEntry));
		field.width = std::max(field.width, entrySize);

		// Store entry
		entries.push_back(newEntry);
	}
	void Table::operator<<(std::vector<const char*> newEntries)
	{
		// Make sure we are starting on the start of a line
		if (entries.size() % fields.size() != 0) return;
		// Make sure the new entries list only has enough entries for one line
		if (newEntries.size() >= fields.size()) return;

		// Loop through each new entry
		for (int i = 0; i < entries.size(); ++i) {
			const char* newEntry = newEntries[i];
			TableField& field    = fields[i];

			// Get the new field width, make sure it isn't wider then the largest unsigned 16 bit int
			uint16_t entrySize = std::min((size_t)((uint16_t)-1), strlen(newEntry));
			field.width = std::max(field.width, entrySize);
		}

		// Copy over entries
		entries.insert(entries.end(), newEntries.end(), newEntries.end());

		// Add padding if there are extra spaces on the current line
		if (newEntries.size() != fields.size()) {
			std::vector<const char*> padding(fields.size() - newEntries.size());
			std::fill(padding.begin(), padding.end(), "");
			entries.insert(entries.end(), padding.end(), padding.end());
		}
	}

	Table CreatTable(const char* name, const std::vector<TableField>& fields, const TableFormat& format)
	{
		Table table;
		table.name = name;
		table.fields = fields;
		table.format = format;

		for (int i = 0; i < fields.size(); ++i) {
			TableField& field = table.fields[i];
			uint16_t fieldSize = std::min((size_t)((uint16_t)-1), strlen(field.name));
			field.width = std::max(field.width, fieldSize);
		}

		return table;
	}

	std::string GenerateTableString(const Table& table)
	{
		size_t tableWidth = 0;
		for (int i = 0; i < table.fields.size(); ++i) {
			tableWidth += (size_t)table.fields[i].width + 3;
		}

		std::string separater;
		{
			std::stringstream outputBuffer;
			outputBuffer << table.format.intersection;
			outputBuffer << std::setfill(table.format.edgeHori);

			for (int i = 0; i < table.fields.size(); ++i) {
				uint16_t width = table.fields[i].width + 3;
				outputBuffer << std::setw(width) << std::right << table.format.intersection;
			}

			separater = outputBuffer.str();
		}
		std::string strightSeparater;
		{
			std::stringstream outputBuffer;
			outputBuffer << table.format.intersection;
			outputBuffer << std::setfill(table.format.edgeHori);
			outputBuffer << std::setw(tableWidth) << table.format.intersection;
			strightSeparater = outputBuffer.str();
		}

		std::stringstream outputBuffer;

		// Push header
		outputBuffer << strightSeparater << std::endl;
		outputBuffer << table.format.edgeVert;
		size_t paddingWidth = (size_t)(ceil(tableWidth / 2) + ceil(strlen(table.name) / 2));
		outputBuffer << std::setw(paddingWidth) << std::right << table.name;
		outputBuffer << std::setw(tableWidth - paddingWidth) << std::right << table.format.edgeVert;
		outputBuffer << std::endl;

		// Push the table fields
		outputBuffer << separater << std::endl;
		outputBuffer << table.format.edgeVert << " ";
		for (size_t i = 0; i < table.fields.size(); ++i) {
			outputBuffer << std::setw(table.fields[i].width) << std::left;
			outputBuffer << table.fields[i].name << " " << table.format.edgeVert << " ";
		}
		outputBuffer << std::endl << separater;

		// Push the table entries
		outputBuffer << std::endl << table.format.edgeVert << " ";
		for (size_t i = 0; i < table.entries.size(); ++i) {

			// Get field of the current entry
			size_t fieldIndex = i % table.fields.size();
			const TableField& field = table.fields[fieldIndex];

			// Push current entry
			outputBuffer << std::setw(field.width) << std::left;
			outputBuffer << table.entries[i] << " " << table.format.edgeVert << " ";

			// Push end line character if this is the last entry on the current line
			if ((i + 1) % table.fields.size() == 0 && i != table.entries.size() - 1) {
				outputBuffer << std::endl << table.format.edgeVert << " ";
			}
			else if (fieldIndex != table.fields.size() - 1 && i == table.entries.size() - 1) {
				for (size_t j = fieldIndex + 1; j < table.fields.size(); ++j) {
					outputBuffer << std::setw((size_t)table.fields[j].width + 1) << std::right;
					outputBuffer << " " << table.format.edgeVert << " ";
				}
				outputBuffer << std::endl;
			}
		}
		outputBuffer << std::endl << separater;

		return outputBuffer.str();
	}
	void PrintTable(const Table& table)
	{
		std::cout << GenerateTableString(table) << std::endl;
	}
}