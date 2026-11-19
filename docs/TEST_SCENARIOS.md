# Validation Scenarios

## 1. Normal operations

- Build and launch from the repository root.
- Verify that the six sample flights, eight crew records and five gates load.
- Run the existing flight lookup, sorting, conflict detection and gate planning options.
- Compare Prim and Kruskal on the same airport network; their minimum total should match.
- Run the advanced lab and compare the four string-matching algorithms on one search phrase; match positions should agree.
- Export CSV and JSON, then import data/flights.json in replace mode and confirm the flight count returns to six.

## 2. Capacity / availability shock

- Make a backup of data/crew.csv.
- Temporarily mark several crew records as assigned or narrow their available_from / available_until windows.
- Restart the app and run 0/1 Knapsack and crew-to-duty-slot Bipartite Matching.
- Compare the number of matched duty slots and the selected priority score with the normal case.
- Restore the original CSV after the demonstration.

## 3. Large-data stress

- Duplicate the sample flight rows with unique flight IDs and plausible route/time values in a separate copy of the CSV.
- Increase the number of records gradually (for example 100, 500, then 1000) and measure sorting, gate planning and string matching.
- Use the benchmark executable for a larger generated manifest; record compiler, machine, input length and timings in the report.
- Keep bounded TSP restricted to small airport networks because its search is exponential.

## Expected interpretation

These are repeatable academic test scenarios, not operational airline validation. Record both input size and assumptions with every timing. The sample dataset is deliberately small and does not represent a live airline schedule.