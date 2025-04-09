import axios from 'axios';
import { useState } from 'react';

const client = axios.create({
    baseURL: process.env.REACT_APP_BASE_URL
});


export const defaultRequest = {
    url: '',
    method: '',
    headers: JSON.stringify({})
};

export function requestReducer(state, action) {
    var targetState={...state};
    switch (action.type) {
        case 'LOADCONFIG':
            targetState.url = '/config';
            targetState.method = 'get';
            //  defaultRequest.body = JSON.stringify(data);
            break;
            case 'STATUS':
                targetState.url = '/status';
                targetState.method = 'get';
                break;
        case 'setColor':
                targetState.url = '/color';
                targetState.method = 'put';
                targetState.data = action.body;
                targetState.params = action.params;
            break;
        case "setBrightness":
            targetState.url = '/brightness';
            targetState.method = 'put';
            targetState.data = action.body;
            targetState.params = action.params;
        break; 
        case "calibrateLdr":
            targetState.url = '/calibrateLdr';  
            targetState.method = 'get';
        break;        
        case "testError":
            targetState.url = '/error';  
            targetState.method = 'get';
        break;       
        case "RESET":
            targetState = {...defaultRequest};
        break;       
        default:
            break;
    }
    return targetState;
}

export default function useRequest(request) {
    const [response, setResponse] = useState(null);
    const [error, setError] = useState({});

    const handleRequest = async (request) => {
        try {

                        var data = await client.request(request);
                        setError({});
            setResponse(data);

        } catch (error) {
            setError(error);
        }

    }

    return [response, error, (request) => handleRequest(request)];
}