all: Main_File

Main_File: FileInputController.o QPConstructor.o Parser.o LineParser.o SQLProcessor.o LogicalQueryPlan.o Process.o LQPGenerator.o LQPOptimizer.o Compare.o ExpressionAnalysis.o TupleOperation.o TupleProcessor.o Search.o AppTokens.o TableProcessor.o StorageManager.o
	g++ -o Main_File FileInputController.o QPConstructor.o Parser.o LineParser.o SQLProcessor.o LogicalQueryPlan.o Process.o LQPGenerator.o LQPOptimizer.o Compare.o ExpressionAnalysis.o TupleOperation.o TupleProcessor.o Search.o AppTokens.o TableProcessor.o StorageManager.o Main_File.cpp

FileInputController.o: FileInputController.h
	g++ -c -std=c++11 FileInputController.cpp

QPConstructor.o: QPConstructor.h Block.h Config.h Disk.h Field.h MainMemory.h Relation.h Schema.h SchemaManager.h Tuple.h RelationSender.h Process.h TupleOperation.h TableProcessor.h TupleProcessor.h
	g++ -c -std=c++11 QPConstructor.cpp

Parser.o: Parser.h Block.h Config.h Disk.h Field.h MainMemory.h Relation.h Schema.h SchemaManager.h Tuple.h DBConfig.h LineParser.h SQLProcessor.h LogicalQueryPlan.h Process.h LQPGenerator.h LQPOptimizer.h
	g++ -c -std=c++11 Parser.cpp

LineParser.o: LineParser.h AppTokens.h AppKeyWords.h
	g++ -c -std=c++11 LineParser.cpp

SQLProcessor.o: SQLProcessor.h Block.h Config.h Disk.h Field.h MainMemory.h Relation.h Schema.h SchemaManager.h Tuple.h LogicalQueryPlan.h
	g++ -c -std=c++11 SQLProcessor.cpp

LogicalQueryPlan.o: LogicalQueryPlan.h Process.h TupleOperation.h QPConstructor.h TableProcessor.h RelationSender.h
	g++ -c -std=c++11 LogicalQueryPlan.cpp

Process.o: Process.h TupleOperation.h Search.h DBConfig.h
	g++ -c -std=c++11 Process.cpp

LQPGenerator.o: LQPGenerator.h LogicalQueryPlan.h Search.h Process.h
	g++ -c -std=c++11 LQPGenerator.cpp

LQPOptimizer.o: LQPOptimizer.h LogicalQueryPlan.h
	g++ -c -std=c++11 LQPOptimizer.cpp

Compare.o: Compare.h ExpressionAnalysis.h
	g++ -c -std=c++11 Compare.cpp

ExpressionAnalysis.o: ExpressionAnalysis.h TupleOperation.h Block.h Config.h Disk.h Field.h MainMemory.h Relation.h Schema.h SchemaManager.h Tuple.h
	g++ -c -std=c++11 ExpressionAnalysis.cpp

TupleOperation.o: TupleOperation.h Block.h Config.h Disk.h Field.h MainMemory.h Relation.h Schema.h SchemaManager.h Tuple.h Process.h DBConfig.h RelationSender.h TableProcessor.h TupleProcessor.h
	g++ -c -std=c++11 TupleOperation.cpp

TupleProcessor.o: TupleProcessor.h Block.h Config.h Disk.h Field.h MainMemory.h Relation.h Schema.h SchemaManager.h Tuple.h DBConfig.h
	g++ -c -std=c++11 TupleProcessor.cpp

Search.o: Search.h Compare.h
	g++ -c -std=c++11 Search.cpp

AppTokens.o: AppTokens.h
	g++ -c -std=c++11 AppTokens.cpp

TableProcessor.o: TableProcessor.h
	g++ -c -std=c++11 TableProcessor.cpp

StorageManager.o: Block.h Disk.h Field.h MainMemory.h Relation.h Schema.h SchemaManager.h Tuple.h Config.h
	g++ -c -std=c++11 StorageManager.cpp

clean:
	rm *.o Main_File
