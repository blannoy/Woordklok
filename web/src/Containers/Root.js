import React, {useState, useEffect,useReducer} from "react";
import Main from "./Main";
import { Provider } from 'react-redux'
import { store } from '../Context/store'

import {configContext, queryProviderContext, clockFaceContext, requestProviderContext} from "../Context/Context";
import {mapClockFace} from "../Utils/ClockFaceMapper";
import useRequest, {requestReducer,defaultRequest } from "../Context/Reducer";

export default function Root() {
    const [initialConfig,setInitialConfig]=useState(null);
    const [clockFace,setClockFace]=useState(null);
    const [error,setError]=useState({currentUrl:"",object:{}});
    const [response, err, runRequest]  = useRequest();
    const [requestState, dispatchRequest] = useReducer(requestReducer, defaultRequest)
    const [responseObject, setResponseObject] = useState(null);

    useEffect(() => {
      if (requestState.url !== ""){
        runRequest(requestState);
      } 
  }, [requestState]);
  
  useEffect(() => {
    if (initialConfig === null && response === null) {
      dispatchRequest({ type: 'LOADCONFIG' });
    } else if (response !== null) {

      switch (response.config.url) {
        case "/config":
          setInitialConfig(response.data);
          //setClockFace(mapClockFace(response.data.clockface));
          dispatchRequest({ type: 'STATUS' });
          break;
          case "/status":
            setResponseObject(response.data);
            break;
        default:
          if (response.config.url !== "/calibrateSensor") {
           if (response.data!==undefined){
             setInitialConfig(response.data);
            }
          }
          break;
      }
    }
    if (err) {
      setError({ currentUrl: window.location.href, object: {...err}});
    }
  }, [response, err])

    return (
      <Provider store={store}>
        <configContext.Provider value={[initialConfig,setInitialConfig]}>
          <clockFaceContext.Provider value={[clockFace,setClockFace]}>
          <queryProviderContext.Provider value={[requestState,dispatchRequest]}>
          <requestProviderContext.Provider value={[responseObject, setResponseObject]}>
            <Main />
            {(Object.keys(error.object).length >0)&&
            <div>
             {(error.currentUrl === window.location.href) &&
             (<pre>
              {error.object.message}<br/>
          {(error.object.response?JSON.stringify(error.object.response.data):"")}</pre>) }
            </div>
            }
            </requestProviderContext.Provider>
            </queryProviderContext.Provider>
            </clockFaceContext.Provider>
        </configContext.Provider>
        </Provider>
    );
}
