import Root from './Containers/Root'
import { createRoot } from 'react-dom/client';
import "./index.css";
import { Provider } from 'react-redux'
import { store } from './Context/store'
import "./i18n";

const container = document.getElementById('root');
const root = createRoot(container); // createRoot(container!) if you use TypeScript

root.render(
  <Provider store={store}>
  <Root/>
  </Provider>
);