## <span id="group-9" style="color: blue">Group 9: Liskov</span>

**Members**: Allie Ianello, Mia Bodenhorn, Mary Holt, Grace Fitzgerald, and Owen Haiar

**Shared Interests**: Games / Productivity

**Notable Strengths**: Algorithms, Databases, Web Development

### Classes to Build

| Class               | Description |
| ------------------- | ----------- |
| **`EventQueue`**    | Track a series of events to trigger based on priority or time point; you should likely use a heap data structure to track which event is next.  A user of the class should be able to request the timing of the next event (peeking at it without removing it), as well and pop the event out of the queue. If two events have the same timing, you should have a consistent method of breaking ties (perhaps by first added or by using secondary value) |
| **`EventManager`**  | A handler for an EventQueue that can handle clock based timings to appropriately trigger events at appropriate times.  An EventManager should also be able to deal with "repeating events" (putting them back in, as appropriate) when triggered, as well as pausing events.  |
| **`Image`**         | A representation of a web image; should maintain the URL of the image, its size, and eventually able to trigger JS code to put it on the web. It should also support alt text (for accessability) and fixed aspect ratios during resizing. |
| **`TextBox`**       | Management object for a web-based version of a presentation text box.  Should be able to hand bullet points, fonts and font-sizes, as both position and size of the box.  One technique you may want use is the have two related functions: `AddText(...)` and `AddRawText(...)`.  The first of these will preserve any special characters (for example, converting a `<` used into a `&lt;` so it still appears correctly.  The second does not alter the text, thus allowing the user to specify any HTML tags they want to use, but needing to appropriately encode things themselves. Other techniques are also possible, such as requiring a `\\` before any tags to indicate that they should not be converted. |
| **`WebLayout`**     | A manager for web pages, including what text or images are on the page and where they should go. Should be able to trigger the appropriate JavaScript code to set up the page and place any images or text boxes, as needed.  You should decide of pages should be static or dynamic and how transitions between layouts should work. |

### Application Suggestion

A **Choose-Your-Own Presentation** manager.  The goal would be to create a simple web-based presentation scripting language where you indicate where text or images should go on each slide.  You should also be able to put buttons on the slides that jump you elsewhere in the slide deck.  This would allow people to easily create dynamic presentations, and people could even create simple games using the system.

Suggested **challenges**: Add extra features for real-time customization, such as entering something in a textbox (like a name or an example animal) that they system stores and can use on later slides.
