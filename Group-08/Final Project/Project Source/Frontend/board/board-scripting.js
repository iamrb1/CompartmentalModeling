import createModule from '../CapstoneProject.js';
let RAS;
let wrapper;
let tagManager;
let dynamicString;
let Card;
let ap;
let board;
let nextId = 1;

createModule().then((Module) => {
  RAS = new Module.RandomAccessSetInt();
  wrapper = new Module.AnnotatedWrapperString();
  ap = new Module.AuditedPointerInt();
  tagManager = new Module.TagManager();
  dynamicString = new Module.DynamicString();
  board = new Module.Board();

  Card = Module.Card;

  const ds = Module.makeDynamicFromString("hello");
  console.log(ds.toString()); // should be "hello"
  ds.delete();

  // Test - create a new card
  const myCard = new Card(1, "Finish Homework");

  console.log("Card ID:", myCard.getId());
  console.log("Card Content:", myCard.getContent());

  myCard.setContent("Updated Task");
  console.log("New Content:", myCard.getContent());

  myCard.clearContent();
  console.log("After Clear:", myCard.getContent());
  // Debugging and testing purposes
  RAS.add(929292929);
  console.log("RAS value test:", RAS.get(0));

  wrapper.addAnnotation("label", "important");
  console.log("Annotation label test:", wrapper.getAnnotation("label"));

  // starts the rest of the app logic
  loadBoardState();
  setupBoard();


  //  Enable editing for NAME/Description
  document.querySelectorAll('.todoList.panel').forEach((stackEl, stackIdx) => {
    enableEditableTitleAndDesc(stackEl, stackIdx);
  });
  

});

function enableEditableTitleAndDesc(stackEl, stackIdx) {
  const titleEl = stackEl.querySelector('h2.todo-text');
  const descEl = stackEl.querySelector('p.panel-card-descr');

  titleEl.contentEditable = true;
  descEl.contentEditable = true;
  const savedTitle = localStorage.getItem(`stack-${stackIdx}-title`);
  const savedDesc = localStorage.getItem(`stack-${stackIdx}-desc`);

  if (savedTitle) titleEl.textContent = savedTitle;
  if (savedDesc) descEl.textContent = savedDesc;

  titleEl.addEventListener('blur', () => {
    const text = titleEl.textContent.trim();
    localStorage.setItem(`stack-${stackIdx}-title`, text);
    saveBoardState();
  });

  descEl.addEventListener('blur', () => {
    const text = descEl.textContent.trim();
    localStorage.setItem(`stack-${stackIdx}-desc`, text);
    saveBoardState();
  });

  [titleEl, descEl].forEach(el => {
    el.addEventListener('keydown', e => {
      if (e.key === 'Enter') {
        e.preventDefault();
        el.blur();
      }
    });
  });
}

let dragged = null;

// CALENDAR DATE
document.addEventListener("DOMContentLoaded", () => {
  const dateEl = document.getElementById("current-date");
  const today = new Date();
  const options = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' };
  dateEl.textContent = today.toLocaleDateString(undefined, options);

  let currentMonth = today.getMonth();
  let currentYear = today.getFullYear();

  function renderCalendar(month, year) {
    const calendar = document.getElementById("monthly-calendar");
    const monthLabel = document.getElementById("month-label");
    calendar.innerHTML = "";

    const firstDay = new Date(year, month, 1).getDay();
    const daysInMonth = new Date(year, month + 1, 0).getDate();

    const monthName = new Date(year, month).toLocaleString(undefined, { month: 'long', year: 'numeric' });
    monthLabel.textContent = monthName;

    const dayNames = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
    dayNames.forEach(day => {
      const dayEl = document.createElement("div");
      dayEl.className = "header";
      dayEl.textContent = day;
      calendar.appendChild(dayEl);
    });

    for (let i = 0; i < firstDay; i++) {
      const emptyCell = document.createElement("div");
      emptyCell.innerHTML = "&nbsp;";
      calendar.appendChild(emptyCell);
    }

    for (let day = 1; day <= daysInMonth; day++) {
      const cell = document.createElement("div");
      const dateObj = new Date(year, month, day);
      const formattedDate = dateObj.toISOString().split("T")[0]; // YYYY-MM-DD
    
      cell.textContent = day;
      cell.classList.add("calendar-day");
      cell.dataset.date = formattedDate;
    
      const isToday =
        day === today.getDate() &&
        month === today.getMonth() &&
        year === today.getFullYear();
    
      if (isToday) {
        cell.classList.add("today");
      }
    
      cell.addEventListener("click", () => {
        document.querySelectorAll(".calendar-day").forEach(el => el.classList.remove("selected"));
        cell.classList.add("selected");
        loadNotesForDate(formattedDate);
      });
    
      calendar.appendChild(cell);
    }
    
  }

  document.getElementById("prev-month").addEventListener("click", () => {
    currentMonth--;
    if (currentMonth < 0) {
      currentMonth = 11;
      currentYear--;
    }
    renderCalendar(currentMonth, currentYear);
  });

  document.getElementById("next-month").addEventListener("click", () => {
    currentMonth++;
    if (currentMonth > 11) {
      currentMonth = 0;
      currentYear++;
    }
    renderCalendar(currentMonth, currentYear);
  });

  renderCalendar(currentMonth, currentYear);
});

// SAVE NOTES LOCALLY
document.addEventListener("DOMContentLoaded", () => {
  const notesArea = document.getElementById("notes-area");
  const saveStatus = document.getElementById("save-status");
  let selectedDate = new Date().toISOString().split("T")[0]; // Default to today

  function loadNotesForDate(date) {
    selectedDate = date;
    const notesKey = `notes-${date}`;
    const timeKey = `notes-time-${date}`;
    const savedNotes = localStorage.getItem(notesKey);
    const savedTime = localStorage.getItem(timeKey);

    notesArea.value = savedNotes || "";
    saveStatus.textContent = savedTime ? `Last saved: ${savedTime}` : "";
  }

  // Load today's notes by default
  loadNotesForDate(selectedDate);

  notesArea.addEventListener("input", () => {
    const now = new Date();
    const timeString = now.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });

    localStorage.setItem(`notes-${selectedDate}`, notesArea.value);
    localStorage.setItem(`notes-time-${selectedDate}`, timeString);
    saveStatus.textContent = `Last saved: ${timeString}`;
  });

  const downloadBtn = document.getElementById("download-notes");
  downloadBtn.addEventListener("click", () => {
    const notesText = notesArea.value || "No notes written yet.";
    const blob = new Blob([notesText], { type: "text/plain" });
    const url = URL.createObjectURL(blob);

    const link = document.createElement("a");
    link.href = url;
    link.download = `notes-${selectedDate}.txt`;
    link.click();

    URL.revokeObjectURL(url); // Clean up
  });

  // Expose loadNotesForDate globally so calendar clicks can call it
  window.loadNotesForDate = loadNotesForDate;
});

// helps prevent duplicate listeners
function addEventListenerOnce(element, event, handler) {
  element.removeEventListener(event, handler);
  element.addEventListener(event, handler);
}

function setupBoard() {
  // DRAG & DROP HANDLERS
  document.querySelectorAll(".draggable").forEach((card) => {
    card.addEventListener("dragstart", (e) => {
      dragged = e.target;
    });

    card.addEventListener("dragend", (e) => {
      e.preventDefault();
      if (dragged && dragged !== e.target) {
        dragged = null;
      }
    });
  });

  document.querySelectorAll(".dropzone").forEach((zone) => {
    zone.addEventListener("dragover", (e) => {
      e.preventDefault();
    });

    zone.addEventListener("drop", (e) => {
      e.preventDefault();
      if (dragged) {
        const panelCards = zone.querySelector(".panel-cards");
        panelCards.appendChild(dragged);
      }
    });
  });

  // CREATE A NEW CARD
  document.querySelector(".top-bar .add-button").addEventListener("click", () => {
    const slider = document.getElementById("slider");
  
    const newColumn = document.createElement("div");
    newColumn.className = "todoList panel dropzone";
  
    newColumn.innerHTML = `
      <div class="panel-card-footer">
                            <h2 class="todo-text" contenteditable="true">Card Name</h2>
                           
                            <label class="due-label">
                          
                              <input type="date" class="task-due" />
                            </label>
                          </div>
      <p class="panel-card-descr" contenteditable="true">Description</p>
  
      <div class="panel-cards"></div>
  
      <div class="panel-card-footer status">
                            <div class="panel-card-add">
                                <div class="input input-stack">
                                    <input type="text" class="input-create" maxlength="30" placeholder="Enter Stack title">
                                    <button class="create-button disabled" disabled>Create Stack</button>
                                </div>
                                <button class="add-button dynamic-add">
                                    <span class="add-button-text">
                                        <span class="add-icon">+</span>
                                    </span>
                                    Add a Tag
                                </button>
                            </div>
                            <label class="status-label">
                                
                              <select class="task-status unstarted">
                                <option>Not Started</option>
                                <option>In Progress</option>
                                <option>Complete</option>
                              </select>
                            </label>
                            
                          </div>
    `;
  
    const titleEl = newColumn.querySelector('.todo-text');
    if (titleEl) {
      titleEl.textContent = "Card Name"; 
    }

    const allStacks = document.querySelectorAll('.todoList.panel');
    const newStackIndex = allStacks.length;
    slider.appendChild(newColumn);
    enableEditableTitleAndDesc(newColumn, newStackIndex);
  
    // manually re-bind dynamic-add to this specific column
    const input = newColumn.querySelector(".input-create");
    const panelCards = newColumn.querySelector(".panel-cards");
    const button = newColumn.querySelector(".dynamic-add");
  
    button.addEventListener("click", () => {
      const cardName = input.value.trim() || "New Tag";
      const cardId = Date.now(); // Use a safe ID
  
      const card = new Card(cardId % 2147483647, cardName);
      board.addCard(card);
      card.addTag(cardName);

      if (tagManager) {
        tagManager.addTag(String(cardId), String(cardName));
      }
  
      const newCard = document.createElement("div");
      newCard.className = "card-wrapper draggable";
      newCard.draggable = true;
      newCard.dataset.cardId = cardId;
  
      newCard.innerHTML = `
        <div class="panel-card card-id">
                                    <p class="panel-card-text">Tag Name</p>

                                    <div class="card-buttons 2">
                                        <button class="edit-button">
                                            <img src="./icons/edit.png" alt="Edit" class="card-icon">
                                        </button>
                                        <button class="delete-button">
                                            <img src="./icons/trash.png" alt="Delete" class="card-icon">
                                        </button>
                                        
                                    </div>
                                </div>
      `;
  
      panelCards.appendChild(newCard);
      input.value = "";
      reloadCardListeners();
    });
  
    reloadCardListeners(); // Make sure it's draggable
  });
  

  // INITIALIZE
  reloadCardListeners();

  // SEARCH FUNCTIONALITY
  function performSearch() {
    const searchTerm = document.querySelector(".search-input").value.trim().toLowerCase();
    const resultsContainer = document.querySelector(".search-results");
    
    // Clear previous highlights and results
    document.querySelectorAll(".card-wrapper").forEach(card => {
      card.style.outline = "";
    });
    resultsContainer.innerHTML = '';
    
    if (!searchTerm) {
      resultsContainer.style.display = 'none';
      return;
    }
  
    const tagMap = new Map();
    
    document.querySelectorAll(".card-wrapper").forEach(card => {
      const cardId = card.dataset.cardId;
      const cardText = card.querySelector(".panel-card-text")?.textContent || "";
      
      // Always include the card's text as a tag
      if (!tagMap.has(cardText)) {
        tagMap.set(cardText, []);
      }
      tagMap.get(cardText).push(cardId);
      
      if (typeof tagManager !== 'undefined') {
        try {
          const tags = tagManager.getTags(cardId);
          for (let i = 0; i < tags.size(); i++) {
            const tagName = tags.get(i);
            if (!tagMap.has(tagName)) {
              tagMap.set(tagName, []);
            }
          }
        } catch (e) {
          console.error("Search error:", e);
        }
      }
    });
  
    // Filter tags that match search (case insensitive)
    const matchingTags = Array.from(tagMap.entries())
      .filter(([tagName]) => tagName.toLowerCase().includes(searchTerm));
  
    if (matchingTags.length > 0) {
      matchingTags.forEach(([tagName, cardIds]) => {
        const resultItem = document.createElement("div");
        resultItem.className = "search-result-item";
        resultItem.textContent = `${tagName} (${cardIds.length})`;
        
        resultItem.addEventListener("click", () => {
          // Highlight all cards with this tag
          document.querySelectorAll(".card-wrapper").forEach(card => {
            card.style.outline = "";
          });
          
          cardIds.forEach(id => {
            const card = document.querySelector(`[data-card-id="${id}"]`);
            if (card) {
              card.style.outline = "2px solid #4285f4";
            }
          });
        });
        
        resultsContainer.appendChild(resultItem);
      });
      resultsContainer.style.display = 'block';
    } else {
      const noResults = document.createElement("div");
      noResults.className = "search-result-item";
      noResults.textContent = "No tags found.";
      resultsContainer.appendChild(noResults);
      resultsContainer.style.display = 'block';
    }
  }
  
  // Update search input event listeners
  document.querySelector(".search-input").addEventListener("input", performSearch);
  
  // Add Enter key support
  document.querySelector(".search-input").addEventListener("keydown", (e) => {
    if (e.key === "Enter") {
      performSearch();
    }
  });
  
  document.querySelector(".search-button").addEventListener("click", performSearch);
  
  // Close dropdown when clicking outside
  document.addEventListener("click", (e) => {
    if (!e.target.closest(".search-container")) {
      document.querySelector(".search-results").style.display = 'none';
    }
  });

  let armedColumn = null;

  document.addEventListener('dblclick', e => {
    const col = e.target.closest('.todoList.panel');
    if (!col) return;

    // remove outline from any previously armed column
    document.querySelectorAll('.todoList.panel.armed')
            .forEach(c => c.classList.remove('armed'));

    col.classList.add('armed');
    armedColumn = col;
  });

  const removeBtn = document.querySelectorAll('.top-bar .add-button')[1];
  removeBtn.addEventListener('click', () => {
    if (!armedColumn) {
      alert('Double‑click the task you want to delete, then press “Remove a Task”.');
      return;
    }

    // 🔧 Clear saved localStorage title/desc for this stack
    const idx = Array.from(document.querySelectorAll('.todoList.panel')).indexOf(armedColumn);
    localStorage.removeItem(`stack-${idx}-title`);
    localStorage.removeItem(`stack-${idx}-desc`);
    
    armedColumn.remove();
    armedColumn = null;
  });

  document.addEventListener('click', e => {
    if (!armedColumn) return;
    if (e.target.closest('.todoList.panel.armed')) return;
    if (removeBtn.contains(e.target)) return;
    armedColumn.classList.remove('armed');
    armedColumn = null;

  });

  saveBoardState();
  loadBoardState();
}

document.addEventListener("click", function(event) {
  const editButton = event.target.closest(".edit-button");
  if (!editButton) return;

  const card = editButton.closest(".panel-card");
  const textElement = card.querySelector(".panel-card-text");

  const isEditable = textElement.isContentEditable;
  textElement.contentEditable = !isEditable;

  if (!isEditable) {
    // Entering edit mode
    textElement.focus();

    const range = document.createRange();
    range.selectNodeContents(textElement);
    const selection = window.getSelection();
    selection.removeAllRanges();
    selection.addRange(range);

    editButton.querySelector("img").src = "./icons/check.png";
  } 
  
  else {
    // Exiting edit mode
    textElement.blur();
    editButton.querySelector("img").src = "./icons/edit.png";
  }
});

// DELETE A CARD
function reloadCardListeners() {
  document.querySelectorAll(".card-wrapper").forEach((card) => {
    // Remove any existing listeners to prevent duplicates
    card.removeEventListener("mouseenter", showCardButtons);
    card.removeEventListener("mouseleave", hideCardButtons);
    
    // Add new listeners
    card.addEventListener("mouseenter", showCardButtons);
    card.addEventListener("mouseleave", hideCardButtons);
  });

  document.querySelectorAll(".delete-button").forEach((button) => {
    button.onclick = (e) => {
      const card = e.target.closest(".card-wrapper");
      if (typeof tagManager !== 'undefined' && card.dataset.cardId) {
        tagManager.clearTagsForTask(card.dataset.cardId);
      }
      card.remove();
      saveBoardState();
    };
  });

  // Allow new cards to be draggable
  document.querySelectorAll(".draggable").forEach((card) => {
    card.addEventListener("dragstart", (e) => {
      dragged = e.target;
    });

    card.addEventListener("dragend", (e) => {
      e.preventDefault();
      dragged = null;
    });
  });
}


// helper functions
function showCardButtons(e) {
  const card = e.currentTarget;
  const buttons = card.querySelector(".card-buttons");
  if (buttons) buttons.style.display = "flex";
}

function hideCardButtons(e) {
  const card = e.currentTarget;
  const buttons = card.querySelector(".card-buttons");
  if (buttons) buttons.style.display = "none";
}


// STORING CARDS
function saveBoardState() {
  const columns = document.querySelectorAll(".todoList.panel");
  const boardData = [];

  columns.forEach((col) => {
    const columnTitle = col.querySelector(".todo-text")?.textContent || "Untitled";
    const dueDate = col.querySelector(".task-due")?.value || "";
    const status = col.querySelector(".task-status")?.value || "Not Started";

    const cards = Array.from(col.querySelectorAll(".card-wrapper")).map(card => ({
      id: card.dataset.cardId,
      content: card.querySelector(".panel-card-text")?.textContent || "Untitled"
    }));

    boardData.push({
      title: columnTitle,
      dueDate,
      status,
      cards
    });
  });

  localStorage.setItem("boardState", JSON.stringify(boardData));
}

function loadBoardState() {
  const saved = localStorage.getItem("boardState");
  if (!saved) return;

  const boardData = JSON.parse(saved);
  const slider = document.getElementById("slider");
  slider.innerHTML = "";

  boardData.forEach((colData, colIndex) => {
    const newColumn = document.createElement("div");
    newColumn.className = "todoList panel dropzone";

    newColumn.innerHTML = `
      <div class="panel-card-footer">
        <h2 class="todo-text" contenteditable="true">${colData.title}</h2>
        <label class="due-label">
          <input type="date" class="task-due" value="${colData.dueDate}" />
        </label>
      </div>
      <p class="panel-card-descr" contenteditable="true">Description</p>
      <div class="panel-cards"></div>
      <div class="panel-card-footer status">
        <div class="panel-card-add">
          <div class="input input-stack">
            <input type="text" class="input-create" maxlength="30" placeholder="Enter Stack title">
            <button class="create-button disabled" disabled>Create Stack</button>
          </div>
          <button class="add-button dynamic-add">
            <span class="add-button-text"><span class="add-icon">+</span></span>
            Add a Tag
          </button>
        </div>
        <label class="status-label">
          <select class="task-status">
            <option ${colData.status === "Not Started" ? "selected" : ""}>Not Started</option>
            <option ${colData.status === "In Progress" ? "selected" : ""}>In Progress</option>
            <option ${colData.status === "Complete" ? "selected" : ""}>Complete</option>
          </select>
        </label>
      </div>
    `;

    const panelCards = newColumn.querySelector(".panel-cards");

    colData.cards.forEach(card => {
      const newCard = document.createElement("div");
      newCard.className = "card-wrapper draggable";
      newCard.draggable = true;
      newCard.dataset.cardId = card.id;

      if (tagManager) {
        tagManager.addTag(card.id, card.content);
      }

      newCard.innerHTML = `
        <div class="panel-card card-id">
          <p class="panel-card-text">${card.content}</p>
          <div class="card-buttons 2">
            <button class="edit-button">
              <img src="./icons/edit.png" alt="Edit" class="card-icon">
            </button>
            <button class="delete-button">
              <img src="./icons/trash.png" alt="Delete" class="card-icon">
            </button>
          </div>
        </div>
      `;

      panelCards.appendChild(newCard);
    });

    slider.appendChild(newColumn);
    enableEditableTitleAndDesc(newColumn, colIndex);
  });

  document.querySelectorAll(".dynamic-add").forEach(button => {
    button.addEventListener("click", function() {
      const column = this.closest(".todoList");
      const input = column.querySelector(".input-create");
      const panelCards = column.querySelector(".panel-cards");

      const cardName = input.value.trim() || "New Tag";
      const cardId = Date.now();

      const card = new Card(cardId % 2147483647, cardName);
      board.addCard(card);
      card.addTag(cardName);

      if (tagManager) {
        tagManager.addTag(String(cardId), String(cardName));
      }

      const newCard = document.createElement("div");
      newCard.className = "card-wrapper draggable";
      newCard.draggable = true;
      newCard.dataset.cardId = cardId;

      newCard.innerHTML = `
        <div class="panel-card card-id">
          <p class="panel-card-text">${cardName}</p>
          <div class="card-buttons 2">
            <button class="edit-button">
              <img src="./icons/edit.png" alt="Edit" class="card-icon">
            </button>
            <button class="delete-button">
              <img src="./icons/trash.png" alt="Delete" class="card-icon">
            </button>
          </div>
        </div>
      `;

      panelCards.appendChild(newCard);
      input.value = "";
      reloadCardListeners();

      document.querySelector(".search-input").dispatchEvent(new Event('input'));
    });
  });

  reloadCardListeners();
}


document.addEventListener("input", saveBoardState);
document.addEventListener("click", (e) => {
  if (e.target.closest(".add-button") || e.target.closest(".delete-button") || e.target.closest(".edit-button")) {
    setTimeout(saveBoardState, 100); 
  }
});
