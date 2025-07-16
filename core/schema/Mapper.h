#pragma once
#include <schema/attributes/Partition.h>

class SchemaMapper
{
public:
  void mapPartitionTo(const Partition &part, char *buffer);
};