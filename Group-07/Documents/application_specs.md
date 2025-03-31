# Team 7 Application: Ecology Simulator

**Description:**   
Runs an ecological simulation using a priority queue to schedule interactions between organisms. The program accepts user-specified experimental parameters for the simulation, including the number of organisms, their initial states, and the types of interactions to be simulated. The simulation runs for a specified duration, and the results are output to a file. 

**Key Functions:**
- Simulation(): Initializes the simulation with user-defined parameters.
- run(): Starts the simulation and manages the event queue.
- processEvent(): Handles the interactions between organisms based on the event queue.

**Error Conditions:**

- Invalid inputs: non-numeric values for the number of organisms or simulation duration. Should raise a ValueError.
- Invalid interaction types: unsupported interaction types specified by the user (such as an animal eating a non-existent animal). Should raise a ValueError.

**Expected challenges:**
- Handling multiple levels of interactions between organisms (e.g. multiple hierarchies of predators and prey).
- Ensuring that the simulation runs efficiently with a large number of organisms and interactions.
- Managing the event queue to ensure that events are processed in the correct order and that no events are missed.

